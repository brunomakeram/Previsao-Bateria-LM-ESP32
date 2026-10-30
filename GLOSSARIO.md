# Glossário

Termos usados em `Calculos.h` (previsão de bateria com regressão linear + otimizador Adam, em C++ para ESP32), na ordem em que aparecem no código. Cada item diz o que é, onde aparece e traz um link para saber mais.

## 1. `#pragma once`
Evita que o mesmo arquivo de cabeçalho seja incluído duas vezes na compilação, o que causaria erro de redefinição.
Aparece na primeira linha do arquivo.
Referência: https://en.cppreference.com/w/cpp/preprocessor/impl

## 2. `#include <Arduino.h>`
Traz as funções e tipos básicos do framework Arduino, usado no ESP32.
Aparece no topo, junto dos outros `#include`.
Referência: https://www.arduino.cc/reference/en/

## 3. `#include <math.h>` (biblioteca de matemática)
Traz funções matemáticas como `sqrtf` e `powf`, usadas no otimizador Adam.
Aparece no topo, junto dos outros `#include`.
Referência: https://en.cppreference.com/w/c/numeric/math

## 4. `inline`
Sugere ao compilador copiar o corpo da função no lugar da chamada, o que evita o custo de chamar a função. Em arquivo de cabeçalho também evita erro de definição repetida.
Aparece em `calcularPrevisao`, `calcularMSE` e `processarAmostraLote`.
Referência: https://en.cppreference.com/w/cpp/language/inline

## 5. `float`
Número decimal de precisão simples (ex.: `0.05f`).
Aparece desde `calcularPrevisao` e em quase todo o arquivo.
Referência: https://en.cppreference.com/w/cpp/language/types

## 6. `const`
Promete que o valor não será alterado dentro da função. Em `const float* pesos`, a função só lê os pesos.
Aparece em `calcularPrevisao` e `processarAmostraLote`.
Referência: https://en.cppreference.com/w/cpp/language/cv

## 7. Ponteiros (`*`)
Variável que guarda o endereço de outra. Em `const float* pesos`, a função recebe o endereço do vetor de pesos, e não uma cópia dele.
Aparece em `calcularPrevisao` (`pesos`) e `processarAmostraLote` (`pesos`, `gradientes`).
Referência: https://en.cppreference.com/w/cpp/language/pointer

## 8. Gradientes
Indicam em que direção e com que intensidade cada parâmetro deve mudar para o erro diminuir. Aqui são acumulados por amostra.
Aparecem na seção "Cálculo de gradientes" e em `processarAmostraLote`.
Referência: https://developers.google.com/machine-learning/crash-course/linear-regression/gradient-descent

## 9. `void`
Indica que a função não devolve valor.
Aparece em `processarAmostraLote` e `atualizarParametros`.
Referência: https://en.cppreference.com/w/cpp/language/types

## 10. Referência (`&` em `float&`)
Apelido para uma variável que já existe. Em `float& somaErroQuadrado`, a função altera a variável original de quem chamou.
Aparece em `processarAmostraLote` (`somaErroQuadrado`) e `atualizarParametros` (`vies`).
Referência: https://en.cppreference.com/w/cpp/language/reference

## 11. Otimizador Adam
Método que atualiza os pesos usando médias móveis dos gradientes (1º momento) e dos gradientes ao quadrado (2º momento). Os valores `beta1`, `beta2` e `eps` vêm do paper original.
Aparece na seção "Otimizador Adam".
Referência: https://arxiv.org/abs/1412.6980

## 12. `struct`
Agrupa variáveis e funções num único tipo. Em `OtimizadorAdam`, guarda a taxa de aprendizado, os momentos e o contador de atualizações.
Aparece em `OtimizadorAdam`.
Referência: https://en.cppreference.com/w/cpp/language/class

## 13. `sqrtf`
Raiz quadrada de um `float`. Usada na correção do segundo momento e no denominador do ajuste.
Aparece em `atualizarParametros`.
Referência: https://en.cppreference.com/w/c/numeric/math/sqrt

## 14. `powf`
Potência de um `float` (base elevada a um expoente). Usada para calcular `beta` elevado ao número de atualizações.
Aparece em `atualizarParametros`, nas correções de viés.
Referência: https://en.cppreference.com/w/c/numeric/math/pow

## 15. Endereço (`&` em `&vies`)
Operador que devolve o endereço de uma variável. Em `{&pesos[0], &pesos[1], &vies}`, monta um vetor de ponteiros para atualizar os três parâmetros no mesmo laço.
Aparece em `atualizarParametros`, na linha `float* parametros[3]`.
Referência: https://en.cppreference.com/w/cpp/language/operator_member_access
