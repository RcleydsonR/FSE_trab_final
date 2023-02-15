# Trabalho Final FSE

| Nome | Matrícula |
| ------------- | ------------- |
| João Pedro Moura Oliveira | 190030879 |
| Flavio Vieira Leao | 150125682 |
| Rafael Cleydson da Silva Ramos | 190019085 |

## Sensores Utilizados

| ESP | Sensor | Portas |
| --- | --- | --- |
|ESP1| Módulo I2C + Display LCD (16x2)| SDA (GPIO 21) e SCL (GPIO 22) |
|ESP1| Módulo Buzzer passivo | ADC1_5 (GPIO 33) |
|ESP2| Módulo DHT11 | GPIO 5 |
|ESP2| Módulo LED 7 Cores | GPIO 19 |
|ESP2| Módulo LED da ESP | GPIO 2 |
|ESP3| Módulo Joystick | X -> ADC1_7 (GPIO 35) e Y -> ADC1_6 (GPIO 34) |
|ESP3| Módulo Sensor de distância ultrassônico | GPIO 33 |
|ESP3| Módulo Buzzer passivo | GPIO 26 |

## Executando

Para executar o código é necessário estar em um ambiente __Espressif__. Para simplificar as explicações, apenas serão documentados os comandos principais

### Edite o menuconfig

Inicialmente, é importante _settar_ as variáveis de ambiente utilizadas no programa. Sendo 4 as principais:

- Modo de operação: (ESP_MODE_CAR, ESP_MODE_MORSE, ESP_MODE_TEMPERATURE)
- ESP_WIFI_SSID: SSID do seu wifi
- ESP_WIFI_PASSWORD: senha do seu wifi
- MQTT_AUTH_TOKEN: auth token da esp obtida pelo thingsboard

```sh
$ idf.py menuconfig
```

### Execute o build da aplicação

```sh
$ idf.py build
```

### Flash e monitoramento

Por fim, após ter buildado corretamente a aplicação execute o comando para dar flash. Opcionalmente é possível monitorar o terminal de saida da esp

```sh
$ idf.py -p /dev/[ttyUSB0] flash monitor
```

## Modo light sleep

Para ativar o modo light sleep disponível no código, abra novamente o menuconfig e altere a variável `ESP_ENERGY_OR_LOW_POWER_MODE` de `ESP_ENERGY_MODE` para `ESP_LOW_POWER_MODE`

```sh
$ idf.py menuconfig

# Execute o build novamente
$ idf.py build

# Rode o flash e monitore
$ idf.py -p /dev/[ttyUSB0] flash monitor
```

## Dashboard

O dashboard para monitoramento do projeto está disponível em: [http://164.41.98.25:443/dashboards/7209aac0-a999-11ed-8436-09bf763c9306](http://164.41.98.25:443/dashboards/7209aac0-a999-11ed-8436-09bf763c9306)

## Apresentação

O vídeo de apresentação está disponível no seguinte link: [https://youtu.be/9sLcW1NbU6U](https://youtu.be/9sLcW1NbU6U)