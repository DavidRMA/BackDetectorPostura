import { Monitor, Coffee, Dumbbell, Eye, Clock, Armchair } from "lucide-react";

const consejos = [
  {
    icon: Monitor,
    titulo: "Pantalla a la altura",
    descripcion: "Coloca tu monitor al nivel de los ojos para evitar tensión cervical.",
    color: "#2563EB",
  },
  {
    icon: Coffee,
    titulo: "Descansos regulares",
    descripcion: "Levántate y estira cada 30-45 minutos para relajar músculos.",
    color: "#F97316",
  },
  {
    icon: Dumbbell,
    titulo: "Fortalece tu espalda",
    descripcion: "Ejercicios simples de fortalecimiento previenen dolores.",
    color: "#22C55E",
  },
  {
    icon: Eye,
    titulo: "Regla 20-20-20",
    descripcion: "Cada 20 min, mira algo a 6 metros durante 20 segundos.",
    color: "#2563EB",
  },
  {
    icon: Clock,
    titulo: "Postura consciente",
    descripcion: "Recuerda revisar tu postura varias veces al día.",
    color: "#F97316",
  },
  {
    icon: Armchair,
    titulo: "Silla ergonómica",
    descripcion: "Ajusta tu silla para mantener los pies en el suelo.",
    color: "#22C55E",
  },
];

export function Consejos() {
  return (
    <div className="space-y-6">
      <h1 className="text-gray-900">Consejos de Bienestar</h1>

      <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
        {consejos.map((consejo, index) => {
          const Icon = consejo.icon;
          
          return (
            <div
              key={index}
              className="bg-white rounded-xl p-6 shadow-sm hover:shadow-md transition-shadow"
            >
              <div
                className="w-12 h-12 rounded-full flex items-center justify-center mb-4"
                style={{ backgroundColor: `${consejo.color}15` }}
              >
                <Icon className="w-6 h-6" style={{ color: consejo.color }} />
              </div>
              <h3 className="text-gray-900 mb-2">{consejo.titulo}</h3>
              <p className="text-gray-600">{consejo.descripcion}</p>
            </div>
          );
        })}
      </div>
    </div>
  );
}