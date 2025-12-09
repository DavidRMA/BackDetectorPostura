// Constantes de la aplicación
export const THINGSPEAK_UPDATE_INTERVAL = 15000; // 15 segundos

export const POSTURE_THRESHOLDS = {
  CHILD: { neutral: 10, mildRisk: 20 },
  TEEN: { neutral: 12, mildRisk: 22 },
  ADULT: { neutral: 15, mildRisk: 25 },
  SENIOR: { neutral: 20, mildRisk: 30 }
};

export const PAGE_NAMES = {
  DASHBOARD: 'dashboard',
  HISTORIAL: 'historial', 
  CONFIGURACION: 'configuracion',
  CONSEJOS: 'consejos'
} as const;