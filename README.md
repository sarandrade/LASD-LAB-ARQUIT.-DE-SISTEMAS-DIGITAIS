# LASD - Laboratório de Arquitetura de Sistemas Digitais

Atividades realizadas na disciplina Laboratório de Arquitetura de Sistemas Digitais no período RAE 2020.3.

## Atividade 1 - Revisão da Linguagem C

**Descrição geral do problema:**
- Implemente um código em C para ordenar um array de 5 elementos.

**Requisitos:**
- Criar um array de inteiros com 5 elementos;
- Povoar o array com 5 números recebidos pelo teclado (dica: scanf());
- Escrever uma função que ordene o array de forma crescente (dica: Bubble sort, Insertion sort, Selection sort ... );
- Retornar para o usuário o array ordenado (dica: printf());
- Estender seu algoritmo para um array de N elementos;
- Empacotar sua função de ordenação em um par de arquivos .h e .c para eventual uso futuro;
- Inicializar seu array com 10000 elementos aleatórios entre 0 e 100 e ordena-lo (dica: rand()%100).

## Atividade 2 - ATmega328p e Primeiro Programa no Atmel Studio 7

**Descrição geral do problema:**
- Implemente um código em C no Atmel Studio 7 que gere um sinal PWM de 5bits por software, utilizando delays. Injete esse sinal no pino PB5, que na placa do Arduino UNO está conectado no LED onboard. Variando o duty cycle do PWM o LED deve aumentar ou diminuir seu brilho.

**Requisitos:**
- Crie uma rotina em C que gere um sinal PWM de 5bits, por software, para acionar o pino PB5 (Led onboard da placa Arduino UNO);
- Escolha o período T=32ms (Freq = 31,25Hz);
- Teste seu programa atribuindo os DutyCycles, 25%, 50% e 75%. Observe a forma do sinal gerado no osciloscópio virtual assim como veja o brilho do led aumentar;
- Crie um laço para que a cada período da onda o DutyCycle seja incrementado de 0 até 100%, repetindo esse ciclo indefinidamente. O brilho do LED deve aumentar gradativamente em seguida apagar, depois voltar a acender gradualmente, apagar...etc.

## Atividade 3 - GPIO

**Descrição geral do problema:** 
- Implemente um código em C no Atmel Studio 7 que execute uma série de animações em uma barra de LEDs. 

**Requisitos:**
- Animação 1: Acenda e apague os leds, em sequência, para parecer que eles estão subindo. Ao chegar no final, reinicie o ciclo automaticamente;
- Animação 2: Acenda e apague os leds, em sequência, para parecer que eles estão descendo. Ao chegar no final, reinicie o ciclo automaticamente;
- Animação 3: Acenda e apague os leds, em sequência, para parecer que eles estão subindo e depois descendo. Ao chegar no final, reinicie o ciclo automaticamente;
- Animação 4: Acenda os leds, em sequência, para parecer que uma barra está crescendo. Ao chegar no final, reinicie o ciclo automaticamente;
- Animação 5: Simule uma bola quicando no chão e perdendo altura em cada iteração. Cada vez que subir novamente a bola deve atingir um quadradinho a menos de altura;
- A qualquer momento o usuário poderá clicar no botão conectado no PD0, que deverá alterar o tipo de animação.

## Atividade 4 - Interrupções

**Descrição geral do problema:**
- Implemente um cronômetro que conte de 0 até 99s com precisão de 1s. O cronômetro deve ter 2 displays de 7 segmentos, 1 botão para start/pause e 1 botão de reset. 

**Requisitos:**
- Capture o start/pause com a interrupção INT0;
- Capture o reset com a interrupção INT1;
- Utilize 2 displays de 7 segmentos para mostrar o tempo;
- A resolução do cronômetro deve ser de 1s. Pode utilizar delays.

## Atividade 5 - Timers

**Descrição geral do problema:**
- Modifique o cronômetro da atividade 4 para gerar sua base de tempo com o Timer0 ao invés de delays.Utilize uma interrupção para contar os segundos de seu cronômetro. O cronômetro deve possibilitar a contagem de 0 até 99s com precisão de 1s. Inclua, no mínimo, 2 displays de 7 segmentos, 1 botão para start/pause e 1 botão de reset.

**Requisitos:**
- Capture o start/pause com a interrupção INT0;
- Capture o reset com a interrupção INT1;
- Utilize 2 displays de 7 segmentos para mostrar o tempo;
- A resolução do cronômetro deve ser de 1s. NÃO utilize delays.

## Atividade 6 - PWM

**Descrição geral do problema:**
- Implemente um código que incremente ou decremente o duty cycle de um sinal PWM gerado pelo Timer0. O duty deve ser incrementado 5% ao clicar um botão e decrementado 5% ao clicar outro. O valor do duty deve ser mostrado em 2 displays de 7 segmentos.

**Requisitos:**
- Gere um sinal PWM no Timer0 com 61Hz;
- Ao clicar um botão o duty cycle do PWM deve incrementar 5%;
- Ao clicar em outro botão o duty cycle do PWM deve decrementar 5%;
- Mostre o valor do duty cycle (0-99%) em 2 displays de 7 segmentos;
- Conecte um LED no pino do PWM;
- Visualize o sinal PWM no osciloscópio.

## Atividade 7 - USART

**Descrição geral do problema:** 
- Implemente uma casa inteligente baseada no Atmega328p. Deverá existir uma lâmpada comum, na sala, acionada por um relé. Uma lâmpada LED com 4 níveis de brilho no quarto e um sensor de presença no terraço (simulado por um botão). A interação com a casa se dará de forma remota através de uma conexão serial via USART e o operador poderá monitorar os equipamentos por um LCD Pcd8544.

**Requisitos:**
- Sala
  - Ao receber pela USART o caractere ‘l’, a lâmpada da sala deve ligar;
  - Ao receber pela USART o caractere ‘d’, a lâmpada da sala deve desligar;
  - Mostre o status da lâmpada da sala no LCD.
- Quarto
  - Ao receber pela USART um número de 0 até 4 a lâmpada de LED do quarto deve acender respectivamente com o brilho de 0%, 25%, 50%, 75% ou 100%. (Dica: PWM);
  - Mostre o percentual do brilho da lâmpada do quarto no LCD.
- Terraço
  - Caso o sensor de presença (um botão) detecte que alguém invadiu o terraço, escreva no display de LCD: “ALARME” por 3 segundos.
- Escritório
  - Automatize também a cortina do escritório, para isso utilize um motor de passo;
  - Ao receber o caractere ‘s’ dê 2 voltas no sentido horário para subir a persiana;
  - Ao receber o caractere ‘c’ dê 2 voltas no sentido anti-horário para descer a persiana;
  - Mostre no LCD se a cortina está levantada, baixada ou em movimento.

## Atividade 8 - ADC

**Descrição geral do problema:**

**Requisitos:**
- Um potenciômetro para ajustar o brilho do LED;
- Mostrar o percentual do brilho em um LCD Pcd8544.

## Atividade 9 - Assembly

**Descrição geral do problema:**
- Implemente um código em Assembly no Atmel Studio 7 que execute uma animação em uma barra de LEDs.

**Requisitos:**
- Acenda e apague os leds, em sequência, para parecer que eles estão subindo. Ao chegar no final, reinicie o ciclo automaticamente;
- O tempo entre um led e outro deve ser de aproximadamente 100ms. Crie uma rotina de delay.

## Projeto Final
