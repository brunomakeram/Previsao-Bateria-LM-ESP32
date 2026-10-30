// Garante que este arquivo seja incluído só uma vez por compilação
#pragma once
// Importa a biblioteca
#include <Arduino.h>

// Etapa 8 Mostrar os resultados: Imprimir os pesos, vieses e o gasto previsto

// Função para mostrar os resultados na tela, incluindo os pesos e vieses aprendidos pelo modelo, bem como o gasto previsto para a viagem
inline void mostrarResultados(float carga, float distancia, float perdas, float pesos[2], float vies, float gasto_previsto) {

  // Mostrar os valores inseridos pelo usuário
  Serial.print("\nValores inseridos: Carga: ");
  Serial.print(carga);
  Serial.print(" kg, Distância: ");
  Serial.print(distancia);
  Serial.println(" km");

  // O MSE (Mean Squared Error) mede a média dos erros ao quadrado
  // RMSE é a raiz do MSE e mostra o erro médio em pontos percentuais de bateria. Ex.: loss de 0.68 → RMSE ≈ 0.82".
  Serial.print("\nLoss final (MSE): ");
  Serial.println(perdas, 6);

  // Mostra a carga com os pesos
  Serial.print("\na (carga kg): ");
  Serial.println(pesos[0], 6);

  // Mostra a distância com os pesos
  Serial.print("b (distância km): ");
  Serial.println(pesos[1], 6);

  // Mostra o viés do modelo
  Serial.print("c (viés): ");
  Serial.println(vies, 6);

  // Mostra o gasto previsto pela IA para os valores inseridos pelo usuário
  Serial.print("\nGasto previsto %: ");
  Serial.println(gasto_previsto, 6);
}
