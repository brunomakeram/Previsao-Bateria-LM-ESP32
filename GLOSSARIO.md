## 5.7 `snprintf`, `sizeof` e `%.9g`
`snprintf` escreve um número formatado dentro de um texto, sem passar do tamanho. `sizeof(buffer)` dá o tamanho do `buffer` em bytes (aqui, 32). `%.9g` usa até 9 dígitos significativos.
Aparece em `formatarNumero`.
Referência (snprintf): https://en.cppreference.com/w/c/io/fprintf
Referência (sizeof): https://en.cppreference.com/w/cpp/language/sizeof

---

# 6. Prints.h

Já explicados antes: `#pragma once` (2.1), `#include <Arduino.h>` (1.1), `inline` (3.1), `void` (1.4), arrays (2.22), `Serial.print` (2.9), `Serial.println` (1.8), `\n` (1.9) e MSE (2.34).

## 6.1 Casas decimais em `Serial.println(valor, 6)`
O segundo argumento é o número de casas decimais que serão mostradas. Ex.: `Serial.println(0.68, 6)` mostra "0.680000".
Aparece em `mostrarResultados`, nos pesos, no viés, na perda e no gasto previsto.
Referência: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/

## 6.2 RMSE
Raiz do MSE. Mostra o erro médio na mesma unidade do dado, aqui em pontos percentuais de bateria. Ex.: MSE de 0.68 dá RMSE ≈ 0.82.
Aparece no comentário de `mostrarResultados`.
Referência: https://en.wikipedia.org/wiki/Root-mean-square_deviation
tilities/progmem/

## 4.3 Literal de texto bruto (`R"CSV( ... )CSV"`)
Texto que vai exatamente como foi escrito, com quebras de linha e sem precisar de `\n`. O `CSV` é só o nome do delimitador.
Aparece no conteúdo de `DATASET_PADRAO`.
Referência: https://en.cppreference.com/w/cpp/language/string_literal

## 4.4 CSV
Formato de arquivo de tabela em texto, com valores separados por vírgula. Aqui tem as colunas `Carga`, `Distância` e `Gasto`, uma amostra por linha.
Aparece no conteúdo de `DATASET_PADRAO` e no arquivo `/Bateria.csv`.
Referência: https://en.wikipedia.org/wiki/Comma-separated_values

---

# 5. Leitura.h

Já explicados antes: `#pragma once` (2.1), `#include "..."` (1.2), `#include <Arduino.h>` (1.1), `#include <vector>` e `std::vector` (2.3), `LittleFS.h` (4.1), `struct` (3.3), `bool` (2.6), `const` e ponteiros (2.4, 2.5), `inline` (3.1), `Serial.println` (1.8), `if` e `return` (2.17), operadores lógicos (2.19), referência `Dataset&` (2.20), `while` (2.10), `String`, `trim()` e `toFloat()` (2.13, 2.15, 2.16), `readStringUntil` (2.14), arrays (2.22), conversão de tipo (2.8) e `+=` (3.2).

## 5.1 Funções do LittleFS (`begin`, `exists`, `open`)
`LittleFS.begin(true)` monta o armazenamento e, com `true`, formata a partição se ela ainda não estiver formatada. `exists` verifica se um arquivo existe e `open` abre um arquivo.
Aparece em `montarArmazenamento`, `lerDataset` e `gravarRegistro`.
Referência: https://github.com/espressif/arduino-esp32/tree/master/libraries/LittleFS

## 5.2 Objeto `File` e modos (`FILE_WRITE`, `FILE_READ`, `FILE_APPEND`)
`File` representa um arquivo aberto. `print` escreve nele, `available` diz se ainda há dados para ler e `close` fecha. `FILE_WRITE` cria ou sobrescreve, `FILE_READ` só lê e `FILE_APPEND` escreve no final sem apagar o que já existe.
Aparece em `montarArmazenamento`, `lerDataset` e `gravarRegistro`.
Referência: https://github.com/espressif/arduino-esp32/tree/master/libraries/FS

## 5.3 `String`: `length()`, `indexOf()`, `substring()`
`length()` dá o tamanho do texto, `indexOf(',', inicio)` acha a posição da próxima vírgula e `substring(inicio, virgula)` recorta o pedaço entre duas posições. Ex.: na linha "10,3,14.56", o primeiro campo é `substring(0, 2)` = "10".
Aparece em `lerDataset`, na divisão da linha em campos.
Referência (length): https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/functions/length/
Referência (indexOf): https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/functions/indexof/
Referência (substring): https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/functions/substring/

## 5.4 `continue` e `break`
`continue` pula para a próxima volta do laço e `break` sai do laço.
Aparece em `lerDataset`: `continue` ignora linhas vazias e pula o cabeçalho, `break` encerra a leitura dos campos da linha.
Referência (continue): https://en.cppreference.com/w/cpp/language/continue
Referência (break): https://en.cppreference.com/w/cpp/language/break

## 5.5 `++` (incremento)
Soma 1 à variável. Em `camposLinha[totalCampos++]`, usa o valor atual como posição e depois soma 1.
Aparece em `lerDataset` e nos laços `for`.
Referência: https://en.cppreference.com/w/cpp/language/operator_incdec

## 5.6 `push_back()` e `empty()`
`push_back` adiciona um elemento no final do vetor e `empty` diz se o vetor está vazio.
Aparece em `lerDataset`: `dados.carga.push_back(...)` e `return !dados.gasto.empty()`.
Referência (push_back): https://en.cppreference.com/w/cpp/container/vector/push_back
Referência (empty): https://en.cppreference.com/w/cpp/container/vector/empty

## 5.7 `snprintf`, `sizeof` e `%.9g`
`snprintf` escreve um número formatado dentro de um texto, sem passar do tamanho. `sizeof(buffer)` dá o tamanho do `buffer` em bytes (aqui, 32). `%.9g` usa até 9 dígitos significativos.
Aparece em `formatarNumero`.
Referência (snprintf): https://en.cppreference.com/w/c/io/fprintf
Referência (sizeof): https://en.cppreference.com/w/cpp/language/sizeof

---

# 6. Prints.h

Já explicados antes: `#pragma once` (2.1), `#include <Arduino.h>` (1.1), `Serial.print()` (2.9), `Serial.println()` (1.8), `float` (2.6), `void` (1.4), `MSE` (2.34) e `\n` (1.9).

## 6.1 `mostrarResultados(...)`
Função responsável por exibir no Monitor Serial os valores inseridos pelo usuário, os pesos aprendidos, o viés e o gasto previsto.
Aparece no final do fluxo de execução, após o cálculo do modelo.
Referência: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/

## 6.2 `Serial.print("\nValores inseridos: ...")`
Texto formatado que mostra os valores de carga e distância antes da previsão final.
Aparece em `mostrarResultados()`.
Referência: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/

## 6.3 `Serial.println(perdas, 6)`
Imprime o valor da perda final com 6 casas decimais, deixando a leitura mais precisa.
Aparece em `mostrarResultados()`, no trecho do MSE final.
Referência: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/

## 6.4 `Serial.println(gasto_previsto, 6)`
Exibe o gasto previsto do modelo com precisão de 6 casas decimais, tornando a previsão mais legível no Monitor Serial.
Aparece no final da função `mostrarResultados()`.
Referência: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/

---

Atualização concluída: o glossário foi reorganizado por arquivo e está alinhado com a estrutura do projeto em `modelo/Iniciar`.
