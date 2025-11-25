from django.db import models

# Create your models here.
class Usuario(models.Model):
    nombre = models.CharField(max_length=100)
    apellido = models.CharField(max_length=100)
    edad = models.IntegerField()
    fecha_registro = models.DateTimeField(auto_now_add=True)
    
    class Meta:
        verbose_name = "Usuario"
        
    def __str__(self):
        return self.nombre

class RegistroPostura(models.Model):
    usuario = models.ForeignKey(Usuario, on_delete=models.CASCADE)
    fechaRegistro = models.DateTimeField(auto_now_add=True)
    duracion = models.DurationField()
    numeroAlertas = models.IntegerField()
    score = models.FloatField()
    
    class Meta:
        verbose_name = "Registro de Postura"

    def __str__(self):
        return f"Registro de {self.usuario.nombre} el {self.fechaRegistro}"