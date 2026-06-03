# 📦 Template Base de Proyecto Python (PEP 8)

Este repositorio es un **template base profesional para proyectos en Python**, siguiendo el estándar **PEP 8**, buenas prácticas modernas y herramientas de calidad de código.

Está pensado para proyectos de:
- Automatización
- Scripts
- APIs (FastAPI / Flask)
- Sistemas embebidos
- Proyectos industriales
- Backend en general

---

## 📁 Estructura del proyecto

```text
mi_proyecto/
├── README.md
├── pyproject.toml
├── .gitignore
├── .env.example
├── requirements.txt
├── src/
│   └── mi_proyecto/
│       ├── __init__.py
│       ├── main.py
│       ├── config.py
│       ├── constants.py
│       ├── utils.py
│       └── services/
│           ├── __init__.py
│           └── sensor_service.py
└── tests/
    ├── __init__.py
    └── test_utils.py
```

---

## 📝 Convenciones de código (PEP 8)

| Elemento     | Convención        | Ejemplo |
|--------------|-------------------|--------|
| Variables    | snake_case        | `sensor_value` |
| Funciones    | snake_case        | `leer_sensor()` |
| Clases       | CamelCase         | `SensorService` |
| Constantes   | MAYÚSCULAS        | `MAX_RETRIES` |
| Archivos     | snake_case.py     | `sensor_service.py` |

---

## 📄 Ejemplo de constantes

```python
MAX_RETRIES = 3
DEFAULT_TIMEOUT = 5.0
SENSOR_OK = 1
SENSOR_ERROR = 0
```

---

## 🧠 Ejemplo de clase

```python
class SensorService:
    def __init__(self, sensor_id: str) -> None:
        self.sensor_id = sensor_id

    def read_value(self) -> float:
        return 23.7
```

---

## ▶️ Ejecución del proyecto

```bash
python -m src.mi_proyecto.main
```

---

## 🧪 Pruebas

Este proyecto utiliza **pytest**:

```bash
pytest
```

---

## ⚙️ Herramientas de calidad de código

### Black (formateador automático)

```bash
black .
```

### Flake8 (validador PEP 8)

```bash
flake8 .
```

---

## 📦 Instalación del entorno

```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

---

## 📄 Configuración (`pyproject.toml`)

```toml
[tool.black]
line-length = 88
target-version = ['py310']

[tool.flake8]
max-line-length = 88
exclude = .git,__pycache__,.venv
```

---

## ✅ Beneficios de este template

✔ Código limpio y consistente  
✔ Fácil de mantener y escalar  
✔ Compatible con equipos y CI/CD  
✔ Alineado con estándares profesionales  

---

## 📌 Recomendaciones

- Usa **Black** para evitar discusiones de formato
- Usa **Flake8** en CI
- Documenta con **docstrings**
- Mantén los módulos pequeños y claros

---

## 📜 Licencia

Este proyecto es de uso libre como template base.
