from rest_framework import serializers
from .models import Usuario, RegistroPostura

class UsuarioSerializer(serializers.ModelSerializer):
    class Meta:
        model = Usuario
        fields = ['id', 'nombre', 'apellido', 'edad', 'fecha_registro']

class RegistroPosturaSerializer(serializers.ModelSerializer):
    usuario = UsuarioSerializer(read_only=True)
    
    class Meta:
        model = RegistroPostura
        fields = ['id', 'usuario', 'fechaRegistro', 'duracion', 'numeroAlertas', 'score']