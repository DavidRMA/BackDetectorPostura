from django.shortcuts import render
from rest_framework import viewsets
from .models import Usuario, RegistroPostura
from .serializer import UsuarioSerializer, RegistroPosturaSerializer
from rest_framework.response import Response
from rest_framework import status
from rest_framework.decorators import action

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
    
    #aqui se deberian definir los endpoins
    
    #endpoit para lectura de registros, ESP envía los datos al Back
    @action(detail=False, methods=['post'], url_path='leer_registro')
    def leer_registro(self, request):
        print("Leyendo registro de postura...")
        print(request.data)
        usuario = Usuario.objects.first()  #enlazar con el primer usuario de la base de datos
        serializer = RegistroPosturaSerializer(data=request.data)
        if serializer.is_valid():
            serializer.save(usuario=usuario)
            print("Registro guardado correctamente.")
            return Response({
                'message': 'Datos guardados correctamente', 
                'data':serializer.data}, status=status.HTTP_201_CREATED)
        return Response({'error':'Error en la lectura del registro enviado por la ESP32'}, serializer.errors, status=status.HTTP_400_BAD_REQUEST)
    
    #endpoint para la comunicacion con el front
    @action(detail=False, methods=['get'], url_path='obtener_registros')
    def listar_registros_usuario(self, request):
        RegistroPostura_queryset = RegistroPostura.objects.all()
        serializer = RegistroPosturaSerializer(RegistroPostura_queryset, many=True)
        print("Listando registros de postura...")
        return Response({
            'message': 'Registros obtenidos correctamente', 
            'data': serializer.data}, status=status.HTTP_200_OK)