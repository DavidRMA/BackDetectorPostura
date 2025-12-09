import { useState, useEffect } from 'react';
import type { HistoryRecord } from '../types/posture';
import { ThingSpeakService } from '../services/thingSpeak';

export function useHistoryData() {
  const [historyData, setHistoryData] = useState<HistoryRecord[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const fetchHistory = async () => {
      try {
        setLoading(true);
        // Por ahora usamos datos de ejemplo
        // Más adelante conectaremos con ThingSpeak
        const exampleData: HistoryRecord[] = [
          { date: "19 Nov 2025", duration: "8h 30m", alerts: 15, score: 82 },
          { date: "18 Nov 2025", duration: "7h 45m", alerts: 12, score: 88 },
          { date: "17 Nov 2025", duration: "6h 20m", alerts: 18, score: 75 },
          { date: "16 Nov 2025", duration: "8h 00m", alerts: 10, score: 90 },
          { date: "15 Nov 2025", duration: "7h 30m", alerts: 14, score: 85 },
        ];
        
        setHistoryData(exampleData);
      } catch (err) {
        setError('Error al cargar el historial');
        console.error('Error fetching history:', err);
      } finally {
        setLoading(false);
      }
    };

    fetchHistory();
  }, []);

  return { historyData, loading, error };
}