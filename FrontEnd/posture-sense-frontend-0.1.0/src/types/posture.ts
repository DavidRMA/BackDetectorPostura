export interface PostureData {
  angleX: number;
  angleY: number;
  maxAngle: number;
  postureLevel: 'NEUTRAL' | 'MILD_RISK' | 'HIGH_RISK';
  state: 'CORRECT' | 'WARNING' | 'BAD_POSTURE' | 'VIBRATING';
  timestamp: Date;
  userAge: number;
  threshold: number;
}

export interface HistoryRecord {
  date: string;
  duration: string;
  alerts: number;
  score: number;
  details?: PostureData[];
}

export interface UserProfile {
  id: string;
  name: string;
  age: number;
  ageGroup: 'CHILD' | 'TEEN' | 'ADULT' | 'SENIOR';
  thresholds: {
    neutral: number;
    mildRisk: number;
  };
  thingSpeakConfig?: {
    channelId: string;
    apiKey: string;
  };
  createdAt: Date;
  updatedAt: Date;
}

export interface PostureThresholds {
  neutral: number;
  mildRisk: number;
  groupName: string;
}