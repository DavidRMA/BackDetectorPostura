from .views import UsuarioViewSet, RegistroPosturaViewSet
from django.urls import path, include
from rest_framework.routers import DefaultRouter

router = DefaultRouter()
router.register(r'usuarios', UsuarioViewSet)
router.register(r'registros_postura', RegistroPosturaViewSet)

urlpatterns = [
    path('', include(router.urls)),
]