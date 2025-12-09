import { useState, useEffect } from 'react';
import type { PostureData } from '../types/posture';
import { ThingSpeakService } from '../services/thingSpeak';
import { useUserProfile } from './useUserProfile';
import { calculatePostureScore } from '../utils/helpers';

export function usePostureData() {
  const [postureData, setPostureData] = useState<PostureData[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const { userProfile } = useUserProfile();

  useEffect(() => {
    const fetchData = async () => {
      try {
        setLoading(true);
        const data = await ThingSpeakService.getLatestData();
        setPostureData(data);
        setError(null);
      } catch (err) {
        setError('Error al cargar datos de postura');
        console.error('Error fetching posture data:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchData();
    
    // Polling cada 15 segundos
    const interval = setInterval(fetchData, 15000);
    return () => clearInterval(interval);
  }, []);

  const currentPosture = postureData[postureData.length - 1];
  
  // Calcular alertas de hoy basadas en el perfil del usuario
  const todayAlerts = postureData.filter(p => {
    const isToday = new Date(p.timestamp).toDateString() === new Date().toDateString();
    const isBadPosture = p.postureLevel === 'HIGH_RISK';
    return isToday && isBadPosture;
  }).length;

  // Calcular score usando la función helper
  const postureScore = calculatePostureScore(postureData);

  return {
    postureData,
    currentPosture,
    todayAlerts,
    postureScore,
    userProfile,
    loading,
    error
  };
}