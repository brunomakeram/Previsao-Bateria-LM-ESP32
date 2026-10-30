# Glossário

Termos usados no código do projeto, organizados por arquivo e na ordem em que aparecem. Cada item diz o que é, onde aparece e traz um link para saber mais. Quando um termo já foi explicado antes, o item só aponta para o número dele (ex.: "ver 2.4").

Arquivos (pasta `modelo/Iniciar`), na ordem do glossário:
1. `Iniciar.ino` (arquivo principal)
2. `Modelo.h`
3. `Calculos.h`
4. `Dataset.h`
5. `Leitura.h`
6. `Prints.h`

---

# 1. Iniciar.ino (arquivo principal)

## 1.1 `#include <Arduino.h>`
Traz as funções e tipos básicos do framework Arduino, usado no ESP32.
Aparece no topo do arquivo e na maioria dos `.h`.
Referência: https://www.arduino.cc/reference/en/

## 1.2 `#include "Arquivo.h"`
Inclui um arquivo do próprio projeto. Aspas procuram na pasta do projeto, e `< >` procuram nas bibliotecas do sistema.
Aparece com `Dataset.h`, `Modelo.h`, `Leitura.h` e `Calculos.h`.
Referência: https://en.cppreference.com/w/cpp/preprocessor/include

## 1.3 `setup()`
Função do Arduino que roda uma única vez ao ligar a placa.
Aparece logo depois dos `#include`.
Referência: https://docs.arduino.cc/language-reference/en/structure/sketch/setup/

## 1.4 `void`
Indica que a função não devolve valor.
Aparece em `setup()`, `loop()`, `executarModelo()` e outras.
Referência: https://en.cppreference.com/w/cpp/language/types

## 1.5 `Serial.begin(115200)`
Inicia a comunicação serial (UART) com o computador. O número é a velocidade em baud e precisa ser o mesmo no Monitor Serial.
Aparece dentro de `setup()`.
Referência: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/

## 1.6 `delay(2000)`
Pausa o programa pelo tempo em milissegundos. Aqui, 2000 ms dão tempo do Monitor Serial abrir.
Aparece em `setup()` (2000 ms), em `loop()` (500 ms) e em `lerNumero` (10 ms).
Referência: https://www.arduino.cc/reference/en/language/functions/time/delay/

## 1.7 `loop()`
Função do Arduino que repete para sempre depois do `setup()`. Aqui, cada volta roda o modelo e espera uma nova consulta.
Aparece depois do `setup()`.
Referência: https://docs.arduino.cc/language-reference/en/structure/sketch/loop/

## 1.8 `Serial.println()`
Envia texto para o Monitor Serial e pula uma linha no final.
Aparece em `loop()`, nas mensagens de "Pronto para uma nova consulta!", e em vários outros arquivos.
Referência: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/

## 1.9 `\n` (quebra de linha)
Sequência de escape que o compilador troca por uma quebra de linha dentro de um texto.
Aparece em `Serial.println("\n====...")` no `loop()`, e também em `readStringUntil('\n')` e nas mensagens de erro.
Referência: https://en.cppreference.com/w/cpp/language/escape

---

# 2. Modelo.h

## 2.1 `#pragma once`
Evita que o mesmo arquivo de cabeçalho seja incluído duas vezes na compilação, o que causaria erro de redefinição.
Aparece na primeira linha de todos os arquivos `.h`.
Referência: https://en.cppreference.com/w/cpp/preprocessor/impl

## 2.2 `#include <math.h>` (biblioteca de matemática)
Traz funções matemáticas como `sqrtf` e `powf`.
Aparece no topo de `Modelo.h` e de `Calculos.h`.
Referência: https://en.cppreference.com/w/c/numeric/math

## 2.3 `#include <vector>` e `std::vector`
`vector` é uma lista que cresce sozinha (substitui as colunas do pandas). O `std::` indica o namespace (grupo de nomes) da biblioteca padrão do C++.
Aparece em `const std::vector<float>& medicoes_carga`, em `std::vector<int> indices(...)` e na struct `Dataset` (Leitura.h).
Referência (vector): https://en.cppreference.com/w/cpp/container/vector
Referência (namespace): https://en.cppreference.com/w/cpp/language/namespace

## 2.4 `const`
Promete que o valor não será alterado depois de definido.
Aparece em `const char* caminho`, `const float cargaMax`, `const int epochs` e outros.
Referência: https://en.cppreference.com/w/cpp/language/cv

## 2.5 Ponteiros (`*`)
Variável que guarda o endereço de outra. Em `const char* caminho = "/Bateria.csv"`, o ponteiro guarda onde começa o texto do caminho do arquivo.
Aparece em `const char* caminho` e também em `Calculos.h` (`pesos`, `gradientes`).
Referência: https://en.cppreference.com/w/cpp/language/pointer

## 2.6 Tipos básicos (`float`, `int`, `bool`, `char`, `long`)
`float` é decimal (ex.: `0.05f`), `int` é inteiro, `bool` é verdadeiro/falso, `char` é um caractere e `long` é um inteiro maior.
Aparecem desde `gerarMumeroAleatorio` e em quase todo o código.
Referência: https://en.cppreference.com/w/cpp/language/types

## 2.7 `esp_random()`
Gera um número inteiro aleatório de 32 bits usando o gerador de hardware do ESP32.
Aparece em `gerarMumeroAleatorio` (pesos iniciais) e no embaralhamento das amostras.
Referência: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/random.html

## 2.8 Conversão de tipo (cast: `(float)`, `(double)`)
Converte um valor de um tipo para outro. Em `gerarMumeroAleatorio`, o inteiro do `esp_random()` vira `double` e é dividido por 4294967295.0. Ex.: 2147483648 / 4294967295.0 ≈ 0.5.
Aparece em `gerarMumeroAleatorio` e em `executarModelo` (`(int)`, `(uint32_t)`).
Referência: https://en.cppreference.com/w/cpp/language/explicit_cast

## 2.9 `Serial.print()`
Envia texto ou número para o Monitor Serial sem pular linha.
Aparece em `lerNumero` e em `Prints.h`.
Referência: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/

## 2.10 `while`
Repete o comando enquanto a condição for verdadeira. Em `lerNumero`, o primeiro `while` esvazia o que já estava no buffer e o segundo espera até o usuário digitar. Sem chaves, o corpo do `while` é só a linha seguinte (para o `delay`, ver 1.6).
Aparece em `lerNumero` e em `lerDataset` (Leitura.h).
Referência: https://en.cppreference.com/w/cpp/language/while

## 2.11 `Serial.available()`
Devolve quantos bytes estão esperando no buffer da Serial. Se for 0, o usuário ainda não digitou nada.
Aparece em `lerNumero`.
Referência: https://www.arduino.cc/reference/en/language/functions/communication/serial/available/

## 2.12 `Serial.read()`
Lê um byte do buffer da Serial. Aqui é usado só para descartar o que sobrou.
Aparece em `lerNumero`.
Referência: https://www.arduino.cc/reference/en/language/functions/communication/serial/read/

## 2.13 `String` (do Arduino)
Tipo de texto do Arduino, com funções prontas como `trim()` e `toFloat()`.
Aparece em `lerNumero` e em `Leitura.h`.
Referência: https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/

## 2.14 `Serial.readStringUntil('\n')`
Lê o que foi digitado até encontrar o caractere informado (aqui, a quebra de linha, ver 1.9).
Aparece em `lerNumero` e em `lerDataset` (via `File`).
Referência: https://www.arduino.cc/reference/en/language/functions/communication/serial/readstringuntil/

## 2.15 `trim()`
Remove espaços e quebras de linha das pontas do texto.
Aparece em `lerNumero` e em `lerDataset`.
Referência: https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/functions/trim/

## 2.16 `toFloat()`
Converte o texto digitado para número decimal. Ex.: "12.5" vira 12.5.
Aparece em `lerNumero` e em `lerDataset`.
Referência: https://www.arduino.cc/reference/en/language/variables/data-types/stringobject/functions/tofloat/

## 2.17 `if` / `else if` / `else` e `return`
Decidem qual bloco rodar. Em `executarModelo`, só roda o treino se a carga e a distância estiverem nos limites. O `return;` sai da função na hora.
Aparece em `executarModelo` e em várias funções de `Leitura.h`.
Referência: https://en.cppreference.com/w/cpp/language/if

## 2.18 `Serial.printf()` e formatos (`%.2f`, `%%`)
Imprime texto formatado. `%.2f` mostra um decimal com 2 casas e `%%` mostra o símbolo `%`. Ex.: `%.2f` com 120.0 mostra "120.00".
Aparece nas mensagens de erro de `executarModelo`.
Referência: https://en.cppreference.com/w/c/io/fprintf

## 2.19 Operadores de comparação e lógicos (`<=`, `==`, `&&`, `!`)
`<=` compara, `==` testa igualdade, `&&` significa "e" e `!` significa "não". Ex.: `!(cargaMin <= carga && carga <= cargaMax)` com carga = 120 e máximo 100: `1.0 <= 120` é verdadeiro, `120 <= 100` é falso, então o `&&` dá falso e o `!` vira verdadeiro (erro).
Aparece em `executarModelo` e em `Leitura.h`.
Referência (comparação): https://en.cppreference.com/w/cpp/language/operator_comparison
Referência (lógicos): https://en.cppreference.com/w/cpp/language/operator_logical

## 2.20 Referência (`&` em tipos)
Apelido para uma variável que já existe, sem copiar. Em `const std::vector<float>& medicoes_carga`, o nome aponta direto para `bateria.carga`.
Aparece em `executarModelo` e em `Calculos.h` (`float&`) e `Leitura.h` (`Dataset&`).
Referência: https://en.cppreference.com/w/cpp/language/reference

## 2.21 `.size()`
Devolve a quantidade de elementos do vetor.
Aparece em `gasto_viagens.size()`.
Referência: https://en.cppreference.com/w/cpp/container/vector/size

## 2.22 Arrays (`float pesos[2]`)
Lista de tamanho fixo. O índice começa em 0. Aqui, `pesos[0]` é o peso da carga e `pesos[1]` é o peso da distância.
Aparece em `pesos[2]` e `gradientes[3]`, e em `Calculos.h` e `Prints.h`.
Referência: https://en.cppreference.com/w/cpp/language/array

## 2.23 Regressão linear (pesos e viés)
O modelo prevê o gasto com uma equação de reta: gasto = a × carga + b × distância + c. Os valores `a` e `b` são os pesos e `c` é o viés.
Aparece na etapa 3 (inicialização dos parâmetros) e em `calcularPrevisao`.
Referência: https://developers.google.com/machine-learning/crash-course/linear-regression

## 2.24 Inicialização Glorot (Xavier)
Jeito de sortear os pesos iniciais dentro de um limite: raiz de 6 dividido pela soma de entradas e saídas. Aqui, `sqrtf(6.0f / (2 + 1))` ≈ 1.41, então cada peso começa entre -1.41 e +1.41.
Aparece na etapa 3, em `limite_glorot`.
Referência: https://keras.io/api/layers/initializers/

## 2.25 Literal `float` (`0.05f`, `1e-7f`)
O `f` no final diz que o número é `float` (e não `double`). `1e-7f` é notação científica: 0.0000001.
Aparece em `inicializar(0.05f, 0.9f, 0.999f, 1e-7f)` e em quase todas as contas.
Referência: https://en.cppreference.com/w/cpp/language/floating_literal

## 2.26 `sqrtf`
Raiz quadrada de um `float`.
Aparece em `limite_glorot` (Modelo.h) e em `atualizarParametros` (Calculos.h).
Referência: https://en.cppreference.com/w/c/numeric/math/sqrt

## 2.27 Otimizador Adam
Método que ajusta os pesos usando médias móveis dos gradientes (1º momento) e dos gradientes ao quadrado (2º momento). Aqui: taxa de aprendizado 0.05, `beta1` 0.9, `beta2` 0.999 e `eps` 1e-7, valores que vêm do paper.
Aparece na etapa 4 (`OtimizadorAdam otimizadorAdam`) e em `Calculos.h`.
Referência: https://arxiv.org/abs/1412.6980

## 2.28 Épocas e lote (mini-batch)
Uma época é uma passada completa por todas as amostras. O lote é o grupo de amostras usado para cada atualização. Aqui são 500 épocas e lotes de 32 amostras.
Aparece na etapa 5 (`epochs`, `tamanho_lote`).
Referência: https://developers.google.com/machine-learning/crash-course/linear-regression/hyperparameters

## 2.29 `for`
Repete um bloco contando de um valor até outro.
Aparece em `executarModelo` (épocas, lotes, índices) e em `Calculos.h`.
Referência: https://en.cppreference.com/w/cpp/language/for

## 2.30 Embaralhamento Fisher-Yates
Algoritmo que embaralha uma lista percorrendo do fim ao começo e trocando cada posição com outra sorteada. Aqui embaralha os `indices` a cada época.
Aparece no começo de cada época.
Referência: https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle

## 2.31 `%` (resto da divisão) e `uint32_t`
`%` dá o resto da divisão. `esp_random() % (i + 1)` sorteia um número de 0 até `i`. `uint32_t` é um inteiro de 32 bits sem sinal.
Aparece no cálculo de `indiceAleatorio`.
Referência (operador): https://en.cppreference.com/w/cpp/language/operator_arithmetic
Referência (tipo): https://en.cppreference.com/w/cpp/types/integer

## 2.32 `min()`
Devolve o menor entre dois valores. Aqui evita que o último lote passe do total de amostras.
Aparece em `fimLote = min(inicioLote + tamanho_lote, numeroTotalAmostras)`.
Referência: https://www.arduino.cc/reference/en/language/functions/math/min/

## 2.33 Gradientes
Indicam em que direção e com que intensidade cada parâmetro deve mudar para o erro diminuir. Aqui são acumulados por amostra e depois divididos pelo tamanho do lote.
Aparecem em `processarAmostraLote` (chamada em `executarModelo`) e em `Calculos.h`.
Referência: https://developers.google.com/machine-learning/crash-course/linear-regression/gradient-descent

## 2.34 MSE (Erro Quadrático Médio)
Média dos erros ao quadrado entre o gasto previsto e o gasto real. Quanto menor, melhor o modelo.
Aparece em `calcularMSE` (`perda_lote`, `perdas`) e em `Prints.h`.
Referência: https://developers.google.com/machine-learning/crash-course/linear-regression/loss

---

# 3. Calculos.h

Já explicados antes: `#pragma once` (2.1), `#include <Arduino.h>` (1.1), `#include <math.h>` (2.2), `const` (2.4), ponteiros (2.5), `float`, `int` e `long` (2.6), `if` e `return` (2.17), referência `float&` (2.20), arrays (2.22), `for` (2.29), `void` (1.4), `sqrtf` (2.26), Adam (2.27), gradientes (2.33) e MSE (2.34).

## 3.1 `inline`
Sugere ao compilador copiar o corpo da função no lugar da chamada, o que evita o custo de chamar a função. Em arquivo de cabeçalho também evita erro de definição repetida.
Aparece em `calcularPrevisao`, `calcularMSE` e `processarAmostraLote`.
Referência: https://en.cppreference.com/w/cpp/language/inline

## 3.2 Atribuição composta (`+=`, `-=`, `/=`)
Atalho para fazer a conta e guardar no mesmo lugar. `x += 2` é igual a `x = x + 2`.
Aparece em `somaErroQuadrado += erro * erro`, `gradientes[i] /= numeroAmostrasLote` e `*(parametros[i]) -= ajuste`.
Referência: https://en.cppreference.com/w/cpp/language/operator_assignment

## 3.3 `struct`
Agrupa variáveis e funções num único tipo. Em `OtimizadorAdam`, guarda a taxa de aprendizado, os momentos e o contador de atualizações, além das funções `inicializar` e `atualizarParametros`.
Aparece em `OtimizadorAdam` (e em `Dataset`, no `Leitura.h`).
Referência: https://en.cppreference.com/w/cpp/language/class

## 3.4 Argumentos padrão (`float lr = 0.05f`)
Valor usado quando quem chama a função não informa o argumento.
Aparece em `inicializar(float lr = 0.05f, float beta1 = 0.9f, ...)`.
Referência: https://en.cppreference.com/w/cpp/language/default_arguments

## 3.5 `powf`
Potência de um `float` (base elevada a um expoente). Usada para calcular `beta` elevado ao número de atualizações. Ex.: na 1ª atualização, `powf(0.999f, 1.0f)` = 0.999.
Aparece em `atualizarParametros`, nas correções de viés.
Referência: https://en.cppreference.com/w/c/numeric/math/pow

## 3.6 Endereço (`&`) e desreferência (`*`)
`&` devolve o endereço de uma variável e `*` vai até o endereço e acessa o valor. Em `{&pesos[0], &pesos[1], &vies}`, monta-se uma lista de endereços, e `*(parametros[i]) -= ajuste` altera o valor original de cada parâmetro no mesmo laço.
Aparece em `atualizarParametros`, na linha `float* parametros[3]`.
Referência: https://en.cppreference.com/w/cpp/language/operator_member_access

---

# 4. Dataset.h

Já explicados antes: `#pragma once` (2.1), `#include <Arduino.h>` (1.1), `#include <vector>` (2.3), `const` (2.4), `char` (2.6) e arrays (2.22).

## 4.1 `#include <LittleFS.h>`
Biblioteca do sistema de arquivos LittleFS, que guarda arquivos na memória flash do ESP32 e mantém os dados depois de reiniciar.
Aparece no topo de `Dataset.h` e de `Leitura.h`.
Referência: https://github.com/espressif/arduino-esp32/tree/master/libraries/LittleFS

## 4.2 `PROGMEM`
Indica que o dado deve ficar na memória flash em vez da RAM. No ESP32 as constantes já costumam ficar na flash, então ele serve mais por compatibilidade com outras placas Arduino.
Aparece em `const char DATASET_PADRAO[] PROGMEM`.
Referência: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/

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

Já explicados antes: `#pragma
