import { useState, useEffect } from 'react';
import type { IUser } from '../models/user';
import { getUsersService, getUserByIdService } from '../services/userService';
import type { UserProfile } from '../types';

export function useUsers() {
  const [users, setUsers] = useState<User[] | null>(null);
  const [loading, setLoading] = useState<boolean>(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    let mounted = true;
    setLoading(true);

    getUsersService()
      .then((data) => {
        if (!mounted) return;
        setUsers(data);
        setError(null);
      })
      .catch((err) => {
        if (!mounted) return;
        setError(err?.message ?? "Error al obtener usuarios");
        setUsers(null);
      })
      .finally(() => {
        if (!mounted) return;
        setLoading(false);
      });

    return () => {
      mounted = false;
    };
  }, []);

  const refresh = async () => {
    setLoading(true);
    try {
      const d = await getUsersService();
      setUsers(d);
      setError(null);
    } catch (e: any) {
      setError(e?.message ?? "Error desconocido");
    } finally {
      setLoading(false);
    }
  };

  return { users, loading, error, refresh };
}

const defaultProfile: UserProfile = {
  id: '1',
  name: 'Usuario',
  age: 25,
  ageGroup: 'ADULT',
  thresholds: {
    neutral: 15,
    mildRisk: 25
  },
  createdAt: new Date(),
  updatedAt: new Date()
};

export function useUserProfile() {
  const [userProfile, setUserProfile] = useState<IUser | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  console.log("Estado actual de userProfile en el hook:", userProfile);

  useEffect(() => {
    const fetchProfile = async () => {
      try {
        const data = await getUserByIdService(1);
        setUserProfile(data);
      } catch (e: any) {
        setError(e?.message || "No se pudo cargar el perfil");
      } finally {
        setLoading(false);
      }
    };

    fetchProfile();
  }, []);

  const updateUserProfile = (updates: Partial<UserProfile>) => {
    setUserProfile(prevProfile => {
      const updatedProfile = {
        ...prevProfile,
        ...updates,
        updatedAt: new Date()
      };
      
      // Calcular grupo de edad y umbrales automáticamente
      if (updates.age !== undefined) {
        const ageGroup = calculateAgeGroup(updates.age);
        const thresholds = getThresholdsByAgeGroup(ageGroup);
        
        updatedProfile.ageGroup = ageGroup;
        updatedProfile.thresholds = thresholds;
      }

      // Guardar en localStorage
      localStorage.setItem('postureCorrectUserProfile', JSON.stringify(updatedProfile));
      
      return updatedProfile;
    });
  };

  return {
    userProfile,
    updateUserProfile,
    loading,
    error
  };
}

function calculateAgeGroup(age: number): UserProfile['ageGroup'] {
  if (age <= 12) return 'CHILD';
  if (age <= 17) return 'TEEN';
  if (age <= 59) return 'ADULT';
  return 'SENIOR';
}

function getThresholdsByAgeGroup(ageGroup: UserProfile['ageGroup']) {
  switch (ageGroup) {
    case 'CHILD':
      return { neutral: 10, mildRisk: 20 };
    case 'TEEN':
      return { neutral: 12, mildRisk: 22 };
    case 'ADULT':
      return { neutral: 15, mildRisk: 25 };
    case 'SENIOR':
      return { neutral: 20, mildRisk: 30 };
    default:
      return { neutral: 15, mildRisk: 25 };
  }
}