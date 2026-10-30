// Importa a biblioteca
#include <Arduino.h>

// Inclui os arquivos do projeto: dados, modelo, leitura e calculos
#include "Dataset.h"
#include "Modelo.h"
#include "Leitura.h"
#include "Calculos.h"

// Roda uma vez ao ligar a placa ESP32
void setup() {
  // Inicia a comunicação serial a 115200 baud
  Serial.begin(115200);
  // Espera 2 segundos pro monitor serial abrir
  delay(2000);
  // Monta o armazenamento e garante que o arquivo CSV exista
  montarArmazenamento(caminho);
}

// Executado continuamente, permitindo novas consultas
void loop() {

  // Roda o modelo a cada ciclo do loop
  executarModelo();
  // Avisa que tá prono pra nova consulta
  Serial.println("\n================================");
  Serial.println("Pronto para uma nova consulta!");
  Serial.println("================================\n");
  // Espera meio segundo antes do próximo ciclo
  delay(500);
}