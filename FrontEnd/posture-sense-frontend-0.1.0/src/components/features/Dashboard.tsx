import { Activity, AlertCircle, TrendingUp } from "lucide-react";
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer } from "recharts";
import { Card } from "../ui/card";
import { usePostureData } from "../../hooks/usePostureData";
import { getPostureLevelColor, getPostureStateColor } from "../../utils/helpers";

export function Dashboard() {
  const { postureData, currentPosture, todayAlerts, postureScore, loading, error } = usePostureData();

  if (loading) {
    return (
      <div className="space-y-6">
        <h1 className="text-2xl font-bold text-gray-900">Dashboard</h1>
        <div className="text-center py-8">Cargando datos del sensor...</div>
      </div>
    );
  }

  if (error) {
    return (
      <div className="space-y-6">
        <h1 className="text-2xl font-bold text-gray-900">Dashboard</h1>
        <div className="text-center py-8 text-red-600">Error: {error}</div>
      </div>
    );
  }

  const posturaCorrecta = currentPosture?.postureLevel === 'NEUTRAL';
  const anguloActual = currentPosture?.maxAngle || 0;

  // Transformar datos para el gráfico (últimas 8 lecturas)
  const chartData = postureData.slice(-8).map((data) => ({
    hora: `${new Date(data.timestamp).getHours()}:${String(new Date(data.timestamp).getMinutes()).padStart(2, '0')}`,
    angulo: data.maxAngle,
    nivel: data.postureLevel
  }));

  return (
    <div className="space-y-6">
      <h1 className="text-2xl font-bold text-gray-900">Dashboard</h1>

      {/* Tarjeta principal de estado */}
      <Card className="p-8">
        <div className="flex items-center justify-between mb-6">
          <div>
            <p className="text-gray-600 mb-2">Estado de la Postura</p>
            <h2 className={`text-3xl font-bold ${getPostureStateColor(currentPosture?.state || 'CORRECT')}`}>
              {currentPosture ? 
                currentPosture.state === 'CORRECT' ? 'Correcta' :
                currentPosture.state === 'WARNING' ? 'Advertencia' :
                currentPosture.state === 'BAD_POSTURE' ? 'Mala Postura' : 'Vibrando'
              : 'Sin datos'}
            </h2>
            {currentPosture && (
              <span className={`inline-flex items-center px-3 py-1 rounded-full text-sm mt-2 ${getPostureLevelColor(currentPosture.postureLevel)}`}>
                {currentPosture.postureLevel === 'NEUTRAL' ? 'Neutral' :
                 currentPosture.postureLevel === 'MILD_RISK' ? 'Riesgo Leve' : 'Alto Riesgo'}
              </span>
            )}
          </div>
          <div className={`w-20 h-20 rounded-full flex items-center justify-center ${
            posturaCorrecta ? "bg-[#22C55E]/10" : "bg-[#EF4444]/10"
          }`}>
            <Activity className={`w-10 h-10 ${
              posturaCorrecta ? "text-[#22C55E]" : "text-[#EF4444]"
            }`} />
          </div>
        </div>
        
        <div className="pt-6 border-t border-gray-200">
          <p className="text-gray-600 mb-1">Ángulo Actual</p>
          <p className="text-2xl font-bold text-gray-900">{anguloActual.toFixed(1)}°</p>
          {currentPosture && (
            <p className="text-sm text-gray-500 mt-1">
              X: {currentPosture.angleX.toFixed(1)}° | Y: {currentPosture.angleY.toFixed(1)}°
            </p>
          )}
        </div>
      </Card>

      {/* Tarjetas de métricas */}
      <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
        <Card className="p-6">
          <div className="flex items-center gap-3">
            <div className="w-12 h-12 rounded-full bg-[#F97316]/10 flex items-center justify-center">
              <AlertCircle className="w-6 h-6 text-[#F97316]" />
            </div>
            <div>
              <p className="text-gray-600">Alertas Hoy</p>
              <p className="text-2xl font-bold text-gray-900">{todayAlerts}</p>
            </div>
          </div>
        </Card>

        <Card className="p-6">
          <div className="flex items-center gap-3">
            <div className="w-12 h-12 rounded-full bg-[#2563EB]/10 flex items-center justify-center">
              <TrendingUp className="w-6 h-6 text-[#2563EB]" />
            </div>
            <div>
              <p className="text-gray-600">Score de Postura</p>
              <p className="text-2xl font-bold text-gray-900">{postureScore}%</p>
            </div>
          </div>
        </Card>
      </div>

      {/* Gráfico */}
      <Card className="p-6">
        <h3 className="text-lg font-semibold text-gray-900 mb-6">Ángulo de Postura - Últimas Lecturas</h3>
        <ResponsiveContainer width="100%" height={300}>
          <LineChart data={chartData}>
            <CartesianGrid strokeDasharray="3 3" stroke="#E5E7EB" />
            <XAxis dataKey="hora" stroke="#6B7280" />
            <YAxis stroke="#6B7280" domain={[0, 90]} />
            <Tooltip 
              formatter={(value: number) => [`${value}°`, 'Ángulo']}
              labelFormatter={(label) => `Hora: ${label}`}
            />
            <Line 
              type="monotone" 
              dataKey="angulo" 
              stroke="#2563EB" 
              strokeWidth={2}
              dot={{ fill: "#2563EB", r: 4 }}
            />
          </LineChart>
        </ResponsiveContainer>
      </Card>
    </div>
  );
}