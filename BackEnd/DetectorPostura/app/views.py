from django.shortcuts import render
from rest_framework import viewsets
from .models import Usuario, RegistroPostura
from .serializer import UsuarioSerializer, RegistroPosturaSerializer
from rest_framework.response import Response
from rest_framework import status
from rest_framework.decorators import action
from datetime import timedelta

# Create your views here.
class UsuarioViewSet(viewsets.ModelViewSet):
    queryset = Usuario.objects.all()
    serializer_class = UsuarioSerializer
    
    @action(detail=False, methods=['get'], url_path='listar_usuarios')
    def listar_usuarios(self, request):
        Usuario_queryset = Usuario.objects.all()
        serializer = UsuarioSerializer(Usuario_queryset, many=True)
        print("Listando usuarios...")
        return Response({
            'message': 'Usuarios obtenidos correctamente', 
            'data': serializer.data}, status=status.HTTP_200_OK)
    
    @action(detail=True, methods=['get'], url_path='consultar_usuario')
    def consultar_usuario(self, request, pk=None):
        try:
            usuario = self.get_object()
            serializer = UsuarioSerializer(usuario)
            return Response({
                'message': 'Usuario obtenido correctamente',
                'data': serializer.data
            }, status=status.HTTP_200_OK)
        except Usuario.DoesNotExist:
            return Response({
                'error': f'Usuario con id {pk} no encontrado'
            }, status=status.HTTP_404_NOT_FOUND)
    
    @action(detail=False, methods=['post'], url_path='crear_usuario')
    def crear_usuario(self, request):
        print("Creando usuario...")
        print(request.data)
        serializer = UsuarioSerializer(data=request.data)
        if serializer.is_valid():
            serializer.save()
            print("Usuario creado correctamente.")
            return Response({
                'message': 'Usuario creado correctamente', 
                'data':serializer.data}, status=status.HTTP_201_CREATED)
        return Response({'error':'Error en la creacion del usuario'}, serializer.errors, status=status.HTTP_400_BAD_REQUEST)
    
class RegistroPosturaViewSet(viewsets.ModelViewSet):
    queryset = RegistroPostura.objects.all()
    serializer_class = RegistroPosturaSerializer

    # endpoint para recepción de datos desde la ESP32
    @action(detail=False, methods=['post'], url_path='leer_registro')
    def leer_registro(self, request):
        print("Leyendo registro de postura desde ESP32...")
        print("Body recibido:", request.data)

        data = request.data

        # Si viene como lista (SenML típico: array de packs)
        if isinstance(data, list):
            if not data:
                return Response(
                    {"error": "Paquete SenML vacío"},
                    status=status.HTTP_400_BAD_REQUEST
                )
            data = data[0]  # tomamos el primer pack

        entries = data.get("e", [])

        def get_value(name, default=None):
            for m in entries:
                if m.get("n") == name:
                    return m.get("v", default)
            return default

        # Extraer valores del paquete
        tilt = get_value("posture/tilt", 0.0)
        bad_posture = bool(get_value("posture/bad_posture", 0))
        threshold = get_value("posture/threshold", 15.0)

        # Métricas simples para el registro:
        numero_alertas = 1 if bad_posture else 0
        # Score 0-100 donde 100 es postura perfecta
        score = max(0.0, 100.0 - float(tilt))

        # De momento asociamos al primer usuario de la BD
        usuario = Usuario.objects.first()
        if not usuario:
            return Response(
                {"error": "No hay usuarios registrados para asociar el registro de postura"},
                status=status.HTTP_400_BAD_REQUEST
            )

        registro = RegistroPostura.objects.create(
            usuario=usuario,
            duracion=timedelta(seconds=1),   # duración de la muestra
            numeroAlertas=numero_alertas,
            score=score,
        )

        serializer = RegistroPosturaSerializer(registro)
        print("Registro guardado correctamente.")
        return Response(
            {
                "message": "Datos de postura recibidos correctamente",
                "data": serializer.data
            },
            status=status.HTTP_201_CREATED
        )

    # endpoint para la comunicación con el front
    @action(detail=False, methods=['get'], url_path='obtener_registros')
    def listar_registros_usuario(self, request):
        registros = RegistroPostura.objects.all()
        serializer = RegistroPosturaSerializer(registros, many=True)
        print("Listando registros de postura...")
        return Response(
            {
                'message': 'Registros obtenidos correctamente',
                'data': serializer.data
            },
            status=status.HTTP_200_OK
        )