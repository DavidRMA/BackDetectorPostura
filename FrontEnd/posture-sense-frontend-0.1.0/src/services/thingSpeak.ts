import type { PostureData, HistoryRecord } from '../types/posture';

export class ThingSpeakService {
  private static getConfig() {
    // Primero intentar desde el perfil del usuario en localStorage
    const savedProfile = localStorage.getItem('postureCorrectUserProfile');
    if (savedProfile) {
      try {
        const profile = JSON.parse(savedProfile);
        if (profile.thingSpeakConfig?.channelId && profile.thingSpeakConfig?.apiKey) {
          return profile.thingSpeakConfig;
        }
      } catch (error) {
        console.error('Error loading ThingSpeak config from profile:', error);
      }
    }

    // Fallback a variables de entorno
    return {
      channelId: import.meta.env.VITE_THINGSPEAK_CHANNEL_ID || '',
      apiKey: import.meta.env.VITE_THINGSPEAK_API_KEY || ''
    };
  }

  static async getLatestData(): Promise<PostureData[]> {
    try {
      const config = this.getConfig();
      
      if (!config.channelId || !config.apiKey) {
        console.warn('ThingSpeak no configurado - usando datos de ejemplo');
        return this.getMockData();
      }

      const response = await fetch(
        `https://api.thingspeak.com/channels/${config.channelId}/feeds.json?api_key=${config.apiKey}&results=50`
      );
      
      if (!response.ok) {
        throw new Error(`Error ${response.status}: ${response.statusText}`);
      }

      const data = await response.json();
      
      if (!data.feeds || data.feeds.length === 0) {
        return this.getMockData();
      }

      return data.feeds.map((feed: any) => ({
        angleX: parseFloat(feed.field1) || 0,
        angleY: parseFloat(feed.field2) || 0,
        maxAngle: parseFloat(feed.field3) || 0,
        postureLevel: this.mapPostureLevel(parseInt(feed.field4)),
        state: this.mapPostureState(parseInt(feed.field5)),
        userAge: parseInt(feed.field7) || 25,
        threshold: parseFloat(feed.field8) || 15,
        timestamp: new Date(feed.created_at)
      }));
    } catch (error) {
      console.error('Error fetching ThingSpeak data:', error);
      return this.getMockData();
    }
  }

  // ... (resto del código se mantiene igual)
  private static mapPostureLevel(level: number): PostureData['postureLevel'] {
    switch(level) {
      case 0: return 'NEUTRAL';
      case 1: return 'MILD_RISK';
      case 2: return 'HIGH_RISK';
      default: return 'NEUTRAL';
    }
  }

  private static mapPostureState(state: number): PostureData['state'] {
    switch(state) {
      case 0: return 'CORRECT';
      case 1: return 'WARNING';
      case 2: return 'BAD_POSTURE';
      case 3: return 'VIBRATING';
      default: return 'CORRECT';
    }
  }

  private static getMockData(): PostureData[] {
    // Datos de ejemplo más realistas
    const now = new Date();
    return [
      {
        angleX: 2.5,
        angleY: 3.1,
        maxAngle: 3.1,
        postureLevel: 'NEUTRAL',
        state: 'CORRECT',
        userAge: 25,
        threshold: 15,
        timestamp: new Date(now.getTime() - 30000)
      },
      {
        angleX: 12.3,
        angleY: 8.7,
        maxAngle: 12.3,
        postureLevel: 'MILD_RISK',
        state: 'WARNING',
        userAge: 25,
        threshold: 15,
        timestamp: new Date(now.getTime() - 15000)
      },
      {
        angleX: 2.1,
        angleY: 4.2,
        maxAngle: 4.2,
        postureLevel: 'NEUTRAL',
        state: 'CORRECT',
        userAge: 25,
        threshold: 15,
        timestamp: now
      }
    ];
  }
}