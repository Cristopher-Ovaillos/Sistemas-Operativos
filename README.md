# DRIVER-xinu-pc-galaga

Driver de teclado para Xinu OS con control de acceso exclusivo mediante semáforos y buffer circular.

## Estructura de Archivos

```
include/teclado_driver.h      → Definiciones y estructuras
device/kbd/
  ├── teclado_driver.c        → Lógica principal
  └── [otros archivos]        → Llaman directamente a los métodos
```

## Componentes Principales (include/teclado_driver.h)

### Buffer Circular (StBuffer stbuffer)
Almacena los scancodes (códigos de teclas presionadas/soltadas) del teclado.

### Semáforo semKbd
Controla que solo un proceso pueda ser el "dueño" del teclado (acceso exclusivo).

### Semáforo stbuffer.semInBf
Sincroniza productor-consumidor: kbdhandler mete datos, el proceso los saca.

### pidKbd
Guarda el PID del proceso que tiene acceso al teclado.

## Flujo de Datos (device/kbd)

### 1. Inicialización
Al inicio del sistema, o cuando llamas a `kbdinit()`:
- Se inicializan los semáforos y el buffer
- Se configura la interrupción del teclado para que llegue a `kbdhandler()`

### 2. Cuando presionas o sueltas una tecla

El hardware del teclado genera una interrupción.

`kbdhandler()` se ejecuta:
1. Lee el scancode del puerto del teclado
2. Llama a `teclado_driver_put_scancode(scancode)` para meter el scancode en el buffer
3. Hace signal al semáforo del buffer para avisar que hay un nuevo dato

**Flujo:**
```
[Presionas tecla] → [IRQ (interrupción)] → [kbdhandler()] 
    → [teclado_driver_put_scancode()] → [buffer + signal(semInBf)]
```

### 3. Cuando un proceso quiere leer una tecla

El proceso debe ser el dueño del teclado (haber hecho open y tener el PID en pidKbd).

Llama a `kbdgetc()` (que llama a `teclado_driver_getc()`):
1. Hace wait sobre el semáforo del buffer
2. Si hay dato disponible, lo saca; si no, espera
3. Devuelve el scancode leído

**Flujo:**
```
[Proceso lector] → [kbdgetc() / teclado_driver_getc()] 
    → [wait(semInBf) → saca scancode]
```

## Acceso Exclusivo con Semáforo

### Apertura
`teclado_driver_open()`:
- Hace `wait(semKbd)` 
- Guarda `getpid()` en pidKbd

Solo un proceso puede leer teclas en ese momento.

### Cierre
Cuando el proceso termina o ya no necesita el teclado, llama a `teclado_driver_close()`:
- Hace `signal(semKbd)`, liberando el recurso para otro proceso

## Uso de open(KEYBOARD, 0, 0)

En Xinu, `open(KEYBOARD, 0, 0)` es la forma estándar de pedir acceso a un dispositivo.

Esto llama internamente a la función `kbdopen()`, que a su vez llama a `teclado_driver_open()`:
- Si el teclado está libre, el proceso se convierte en dueño
- Si está ocupado, el proceso se bloquea hasta que esté libre

Garantiza que solo un proceso tiene acceso exclusivo al teclado a la vez.

## Ejemplo de Uso en Proceso Xinu

```c
open(KEYBOARD, 0, 0); // Pide acceso exclusivo al teclado

unsigned char sc = kbdgetc(); // Espera y lee el próximo scancode

close(KEYBOARD); // Libera el teclado
```
