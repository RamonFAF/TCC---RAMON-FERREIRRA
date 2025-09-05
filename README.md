# TCC-RAMON-FERREIRRA

Este projeto implementa um controle de fase utilizando o microcontrolador ESP8266, desenvolvido como parte do Trabalho de Conclusão de Curso em Engenharia Elétrica – IFPE, Campus Pesqueira.
O sistema é capaz de disparar um TRIAC em ângulos variáveis, controlando a potência entregue à carga com base na leitura de um potenciômetro.

🔧 Hardware Utilizado

ESP8266 (NodeMCU / Wemos D1 Mini ou similar)

TRIAC (ex.: BT136, BT151) com circuito de disparo isolado

Potenciômetro B10k conectado entre 3V3 – A0 – GND

Detecção de passagem por zero via optoacoplador (ex.: H11AA1)

Carga resistiva (lâmpada incandescente, resistor de potência, etc.)

Ligações principais:

Zero-Cross → D7 (GPIO13)

Gate TRIAC → D6 (GPIO12)

Potenciômetro → A0 (ADC do ESP8266)

📜 Funcionamento do Código

O sinal de passagem por zero é detectado por interrupção (attachInterrupt).

O período da rede elétrica é medido em microssegundos (micros()), garantindo sincronismo.

O atraso de disparo (ângulo de fase) é calculado proporcionalmente ao valor lido no potenciômetro (0–1023).

O disparo do gate é agendado com Ticker.once(), que garante precisão no tempo de ativação.

O pulso de gate dura 1000 µs (ajustável via GATE_PULSE_US).

São exibidas mensagens de depuração na Serial Monitor (115200 baud) mostrando:

Período detectado

Valor do potenciômetro

Tempo de atraso

Ângulo correspondente (0° a 180°)

⚠️ Segurança

Atenção: Este projeto envolve tensão da rede elétrica (110/220 VCA).

Trabalhe sempre com isolamento adequado (transformador, optoacopladores, fusíveis e DDR).

Não toque em nenhuma parte do circuito quando energizado.

Utilize cargas resistivas para os testes iniciais.

Recomendado uso em bancada didática com supervisão.

▶️ Como Executar

Clone este repositório:

git clone https://github.com/seuusuario/controle-fase-esp8266.git


Abra o código no Arduino IDE (ou PlatformIO).

Selecione a placa NodeMCU 1.0 (ESP-12E) ou equivalente.

Compile e faça upload.

Abra o Serial Monitor (115200 baud) para acompanhar os valores.

📈 Exemplo de Saída Serial
Per=10000 us  pot=512  atraso=4950 us  Ângulo=89.1
Per=10000 us  pot=800  atraso=7720 us  Ângulo=139.5
Per=10000 us  pot=100  atraso=950 us   Ângulo=17.2

📚 Referências

Datasheet ESP8266

Datasheet TRIAC BT151

Datasheet Optoacoplador 4N35

Ticker Library – Arduino
