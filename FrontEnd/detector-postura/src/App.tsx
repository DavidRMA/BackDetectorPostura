import { BrowserRouter, Routes, Route } from 'react-router-dom';
import MainLayout from './layout/mainLayout';

import Dashboard from './pages/dashboard';
import Historial from './pages/historial';
import Configuracion from './pages/configuracion';
import Consejos from './pages/consejos';

function App() {
  return (
    <BrowserRouter>
      <MainLayout>
        <Routes>
          <Route path='/' element={<Dashboard />} />
          <Route path='/historial' element={<Historial />} />
          <Route path='/configuracion' element={<Configuracion />} />
          <Route path='/consejos' element={<Consejos />} />
        </Routes>
      </MainLayout>
    </BrowserRouter>
  );
}

export default App
