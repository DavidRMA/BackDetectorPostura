import create from 'zustand';
import { IUser } from '../models/user';
import { getUserByIdService} from '../services/userService';

interface UserState {
  user: IUser | null;
  loading: boolean;
  error: string | null;
  fetchUser: () => Promise<void>;
  setUser: (user: IUser) => void;
}

export const useUserStore = create<UserState>((set) => ({
  user: null,
  loading: true,
  error: null,
  fetchUser: async () => {
    set({ loading: true, error: null });
    try {
      const user = await getUserByIdService();
      set({ user, loading: false });
    } catch (err: any) {
      set({ error: err?.message ?? "Error al obtener usuario", loading: false });
    }
  },
  setUser: (user) => set({ user }),
}));