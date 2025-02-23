import socket
import pyautogui
import time

# Configuração de rede
UDP_IP = "0.0.0.0"
UDP_PORT = 5005
BITDOGLAB_IP = "192.168.18.17"  # IP da sua BitDogLab (ajuste conforme necessário)

def notify_bitdoglab():
    """Envia um pacote UDP para ativar o modo Mouse Wi-Fi."""
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(b"start_mouse_mode", (BITDOGLAB_IP, UDP_PORT))
    sock.close()
    print("Modo Mouse Wi-Fi ativado na BitDogLab.")


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
sock.setblocking(False)  # Torna o socket não bloqueante

print(f"Servidor UDP rodando na porta {UDP_PORT}...")

# Notifica a BitDogLab no início
notify_bitdoglab()

last_x, last_y = 0, 0  # Últimos valores recebidos
moving = False  # Indica se o joystick está movendo

while True:
    try:
        data, addr = sock.recvfrom(1024)  # Tenta receber dados sem bloquear
        decoded_data = data.decode().strip()

        if decoded_data == "STOP":
            print("Joystick parado, interrompendo movimento.")
            moving = False
        elif decoded_data == "ButtonA":
            print("Botão A pressionado.")
            pyautogui.click()
        elif decoded_data == "ButtonB":
            print("Botão B pressionado.")
            pyautogui.rightClick()
        elif decoded_data == "ButtonJ":
            print("Botão J pressionado.")
            pyautogui.middleClick()
        else:
            try:
                last_x, last_y = map(int, decoded_data.split(","))
                print(f"Recebido de {addr}: X={last_x}, Y={last_y}")
                moving = True  # Indica que há movimento ativo
            except ValueError:
                print(f"Dados inválidos recebidos: {decoded_data}")

    except BlockingIOError:
        pass  # Nenhum dado recebido, continua o loop

    if moving:
        # Move de forma relativa (incremento) ao invés de ir para uma posição fixa
        pyautogui.moveRel(last_x, -last_y, 0.1)  # Inverte Y para alinhar com a tela

    time.sleep(0.0005)  # Pequena pausa para evitar uso excessivo da CPU
