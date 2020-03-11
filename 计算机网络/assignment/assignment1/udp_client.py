import socket

def server():
    host = '127.0.0.1'
    port = 10000
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.bind((host,port))
        
        while True:
            data, addr = s.recvfrom(1024)
            if not data:
                break
            data = data.upper()
            s.sendto(data, addr)

def client():
    host = '127.0.0.1'
    port = 10000
    inputs = input("Input lowercase sentence:")
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.sendto(inputs.encode(), (host,port))
        data, addr = s.recvfrom(1024)

    print('received',data.decode())

if __name__ == "__main__":
    client()