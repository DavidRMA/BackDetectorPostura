import { useState } from "react";
import { Header } from "./components/layout/Header";
import { Sidebar } from "./components/layout/Sidebar";
import { Dashboard } from "./components/features/Dashboard";
import { Historial } from "./components/features/Historial";
import { Configuracion } from "./components/features/Configuracion";
import { Consejos } from "./components/features/Consejos";

type Page = "dashboard" | "historial" | "configuracion" | "consejos";

export default function App() {
  const [currentPage, setCurrentPage] = useState<Page>("dashboard");

  const renderPage = () => {
    switch (currentPage) {
      case "dashboard":
        return <Dashboard />;
      case "historial":
        return <Historial />;
      case "configuracion":
        return <Configuracion />;
      case "consejos":
        return <Consejos />;
      default:
        return <Dashboard />;
    }
  };

  return (
    <div className="min-h-screen bg-[#F5F7FB]">
      <Header />
      <Sidebar currentPage={currentPage} onPageChange={setCurrentPage} />
      
      <main className="ml-64 mt-16 p-8">
        <div className="max-w-7xl mx-auto">
          {renderPage()}
        </div>
      </main>
    </div>
  );
}
