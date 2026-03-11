// Parámetros de la señal ECG
// Array de datos ECG (el compilador contará automáticamente)

const int ecg_data[] = {
  0,13,25,37,47,57,66,73,80,85,89,93,95,96,97,97,96,96,94,93,92,90,89,88,87,86,85,85,85,84,84,85,85,86,86,87,88,89,91,92,93,94,95,96,97,97,98,98,98,99,99,99,99,99,99,99,99,99,99,98,98,98,97,97,96,95,95,94,94,93,93,92,92,91,91,90,90,89,89,88,88,87,86,86,85,85,84,84,83,83,83,83,82,82,82,81,81,80,80,80,79,79,79,79,78,78,78,78,78,78,77,77,77,77,76,76,76,75,75,75,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,75,75,74,74,74,73,72,71,69,67,66,63,61,59,56,54,51,49,46,44,41,39,38,37,36,37,38,40,43,47,52,58,65,73,83,93,104,115,127,140,152,165,177,189,201,212,222,231,239,245,250,253,255,255,254,250,246,239,232,224,214,204,193,182,171,160,149,138,128,118,109,100,93,86,80,75,70,67,64,62,61,61,61,61,62,64,65,67,68,70,72,74,76,77,79,81,82,84,85,86,87,88,89,89,89,89,89,89,89,88,88,87,87,87,86,86,86,85,85,85,84,84,84,83,83,83,83,83,83,83,83,83,83,84,84,84,85,85,85,85,86,86,86,86,86,87,87,88,88,89,89,90,90,91,91,91,92,92,92,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,104,104,105,106,107,108,108,109,110,111,111,112,113,113,114,115,116,117,117,118,119,120,121,122,123,123,124,125,126,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,141,142,143,144,145,146,147,148,149,150,151,152,152,153,154,155,155,156,157,157,158,159,159,160,161,162,162,163,164,164,165,165,165,166,166,166,166,166,166,166,166,166,166,166,165,165,164,163,163,162,161,160,159,158,157,156,155,154,153,152,150,149,148,147,145,144,142,141,139,138,137,135,134,133,131,130,129,128,126,125,123,122,121,119,118,116,115,114,112,111,110,109,108,107,106,105,104,102,101,100,99,98,97,96,95,94,93,92,92,91,90,90,89,88,87,87,86,85,84,84,83,83,82,82,82,81,81,80,80,80,79,79,78,78,77,77,77,76,76,76,76,75,75,75,75,75,74,74,74,73,73,73,72,72,72,72,72,72,71,71,71,71,71,71,71,71,70,70,70,70,70,70,70,70,70,70,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,72,72,72,72,72,72,72,71,71,71,71,71,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,73,73,73,74,74,75,75,76,76,77,77,77,77,77,77,77,77,76,76,75,74,73,71,70,68,67,65,63,61,59,57,55,53

};

// El tamaño se calcula automáticamente
const int ECG_VALUES = sizeof(ecg_data) / sizeof(ecg_data[0]);
// Pines
const int DAC_PIN = 25;      // GPIO25 (DAC1)
const int BUTTON_PIN = 26;   // GPIO26 para el pulsador (cambia si lo necesitas)

// Variables de frecuencia
float current_frequency = 1.0;  // Frecuencia actual en Hz (1Hz por defecto)
float period_ms;                // Periodo actual en ms
unsigned long last_sample_time = 0;
unsigned long last_button_time = 0;
const unsigned long debounce_delay = 50;  // Delay anti-rebote

// Variables de control
int sample_index = 0;
bool frequency_toggle = false;  // false = 1Hz, true = 2Hz

void setup() {
  Serial.begin(115200);
  
  // Configurar pines
  pinMode(DAC_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Pulsador con resistencia pull-up interna
  
  // Calcular periodo inicial
  update_period();
  
  Serial.println("Generador de señal ECG iniciado");
  Serial.println("Frecuencia inicial: 1 Hz");
  Serial.println("Presiona el pulsador para cambiar a 2 Hz");
  Serial.println("Presiona nuevamente para volver a 1 Hz");
}

void loop() {
  unsigned long current_time = millis();
  
  // Control del pulsador (toggle)
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (current_time - last_button_time > debounce_delay) {
      frequency_toggle = !frequency_toggle;
      
      if (frequency_toggle) {
        current_frequency = 2.0;
        Serial.println("Frecuencia cambiada a: 2 Hz");
      } else {
        current_frequency = 1.0;
        Serial.println("Frecuencia cambiada a: 1 Hz");
      }
      
      update_period();
      last_button_time = current_time;
      
      // Pequeña pausa para evitar múltiples detecciones
      delay(200);
    }
  }
  
  // Generación de la señal ECG
  if (current_time - last_sample_time >= period_ms) {
    // Obtener valor ECG actual (ajustar si hay valores negativos)
    int ecg_value = ecg_data[sample_index];
    
    // Asegurar que el valor esté en el rango 0-255 para DAC de 8 bits
    if (ecg_value < 0) {
      ecg_value = 0;
    } else if (ecg_value > 255) {
      ecg_value = 255;
    }
    
    // Escribir en el DAC (GPIO25)
    dacWrite(DAC_PIN, ecg_value);
    
    // Actualizar índice circular
    sample_index++;
    if (sample_index >= ECG_VALUES) {
      sample_index = 0;
    }
    
    last_sample_time = current_time;
  }
}

void update_period() {
  // Calcular periodo en milisegundos
  // T = 1/f * 1000 (ms)
  // Dividimos entre el número de muestras para obtener el tiempo por muestra
  period_ms = (1000.0 / current_frequency) / ECG_VALUES;
  
  Serial.print("Periodo por muestra: ");
  Serial.print(period_ms, 4);
  Serial.println(" ms");
}