import type { PostureData } from '../types/posture';

// Utilidades generales
export function cn(...classes: (string | undefined | boolean)[]) {
  return classes.filter(Boolean).join(' ');
}

export function formatDate(date: Date): string {
  return new Intl.DateTimeFormat('es-ES', {
    day: '2-digit',
    month: '2-digit',
    year: 'numeric'
  }).format(date);
}

export function formatTime(date: Date): string {
  return new Intl.DateTimeFormat('es-ES', {
    hour: '2-digit',
    minute: '2-digit'
  }).format(date);
}

export function calculatePostureScore(postureData: PostureData[]): number {
  if (postureData.length === 0) return 0;
  
  const goodPostureCount = postureData.filter(
    data => data.postureLevel === 'NEUTRAL'
  ).length;
  
  return Math.round((goodPostureCount / postureData.length) * 100);
}

export function getPostureLevelColor(level: PostureData['postureLevel']): string {
  switch (level) {
    case 'NEUTRAL':
      return 'text-green-600 bg-green-100';
    case 'MILD_RISK':
      return 'text-orange-600 bg-orange-100';
    case 'HIGH_RISK':
      return 'text-red-600 bg-red-100';
    default:
      return 'text-gray-600 bg-gray-100';
  }
}

export function getPostureStateColor(state: PostureData['state']): string {
  switch (state) {
    case 'CORRECT':
      return 'text-green-600';
    case 'WARNING':
      return 'text-orange-600';
    case 'BAD_POSTURE':
      return 'text-red-600';
    case 'VIBRATING':
      return 'text-red-600 animate-pulse';
    default:
      return 'text-gray-600';
  }
}

export function calculateDuration(start: Date, end: Date): string {
  const diffMs = end.getTime() - start.getTime();
  const hours = Math.floor(diffMs / (1000 * 60 * 60));
  const minutes = Math.floor((diffMs % (1000 * 60 * 60)) / (1000 * 60));
  
  return `${hours}h ${minutes}m`;
}