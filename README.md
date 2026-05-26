# Proyecto Final SO: Parche CVE-2025-12744 (Fedora ABRT)

[![UNAM - Ciencias](https://img.shields.io/badge/UNAM-Facultad_de_Ciencias-blue.svg)](https://www.fciencias.unam.mx/)
[![Vulnerability - Command Injection](https://img.shields.io/badge/Vulnerability-Command_Injection-red.svg)](https://www.exploit-db.com/exploits/52515)

## 📌 Descripción del Proyecto
Este repositorio contiene la auditoría, análisis y parche de seguridad para la vulnerabilidad **CVE-2025-12744** (EDB-ID: 52515) encontrada en el componente ABRT (*Automatic Bug Reporting Tool*) del sistema operativo Fedora. 

El proyecto fue desarrollado como entrega final para el Laboratorio de Sistemas Operativos, demostrando la explotación de una falla arquitectónica en la creación de procesos y su respectiva mitigación a nivel de código fuente.

### La Vulnerabilidad Original
La falla radica en una **inyección de comandos** dentro de la función `dump_docker_info()` en el archivo `abrt-action-save-container-data.c`. El sistema procesaba el `container_id` proporcionado por el usuario y lo concatenaba directamente en una cadena de texto para ser ejecutado mediante `/bin/sh -c`. Al no existir sanitización, un atacante local podía inyectar metacaracteres de terminal (como `;`) para lograr una **escalada local de privilegios**.

### La Solución Implementada
Se modificó la arquitectura de ejecución. Se eliminó el uso inseguro del intérprete de comandos (*shell*) basado en cadenas de texto, sustituyéndolo por un flujo seguro que utiliza un arreglo estructurado de argumentos (`char *args[]`) pasado directamente a la llamada del sistema `execvp()`. Esto obliga al kernel a tratar la entrada del usuario estrictamente como un literal de texto, neutralizando cualquier intento de inyección.

---

## 📂 Estructura del Repositorio

* `abrt-action-save-container-data-vulnerable.c`: Código fuente original extraído de Fedora que contiene la vulnerabilidad.
* `abrt-action-save-container-data-patched.c`: Código fuente con nuestro parche de seguridad implementado.
* `poc_vulnerable.c`: Prueba de Concepto (PoC) en C estándar que aísla y simula el fallo lógico de diseño para sistemas no basados en Red Hat (ej. Ubuntu).
* `poc_patched.c`: Prueba de Concepto (PoC) que demuestra la efectividad de la solución basada en `execvp()` en cualquier ecosistema Linux.

---

## ⚙️ Instrucciones de Reproducción y Uso

El proyecto está diseñado para ser evaluado en dos partes: un entorno nativo (Fedora) y un entorno abstracto/simulado (Ubuntu u otras distribuciones).

### 1. Entorno Nativo (Fedora VM)
Para compilar y auditar el código real del componente ABRT, se requieren las librerías dinámicas del ecosistema de Red Hat.

**Dependencias:**
```bash
sudo dnf install libreport-devel json-c-devel glib2-devel gcc
```

**Compilación:**

```bash

# Compilar código vulnerable
gcc -o abrt-vulnerable abrt-action-save-container-data-vulnerable.c -lreport -ljson-c

# Compilar código parcheado
gcc -o abrt-patched abrt-action-save-container-data-patched.c -lreport -ljson-c
```

### 2. Prueba de Concepto Multiplataforma (Ubuntu/Debian)
Dado que las librerías de libreport son exclusivas de Fedora, proporcionamos un PoC en C estándar para demostrar la vulnerabilidad lógica y la mitigación en cualquier distribución.

**Compilación:**

```bash
gcc -o poc_vulnerable poc_vulnerable.c
gcc -o poc_patched poc_patched.c
```

Ejecución del Exploit (Prueba de Inyección):

```bash
# 1. Atacar el entorno vulnerable:
./poc_vulnerable "12345; cat /etc/passwd"
# RESULTADO: El sistema colapsa y ejecuta el comando 'cat', exponiendo información sensible.

# 2. Atacar el entorno parcheado:
./poc_patched "12345; cat /etc/passwd"
# RESULTADO: El sistema bloquea el ataque e imprime la carga útil como texto inofensivo.
```

### 👥 Equipo de Trabajo
Sistemas Operativos - Facultad de Ciencias, UNAM (Semestre 2026-2).

* Flores Cid Marco

* Rodríguez Nevarez Edwin

### 📚 Referencias
* Exploit-DB. Fedora - Local Privilege Escalation. EDB-ID: 52515. https://www.exploit-db.com/exploits/52515

* National Vulnerability Database. CVE-2025-12744. https://nvd.nist.gov/vuln/detail/CVE-2025-12744