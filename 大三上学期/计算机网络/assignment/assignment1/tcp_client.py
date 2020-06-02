import socket

def server():
    host = '127.0.0.1'
    port = 10000
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host,port))
        s.listen(5)
        conn, addr = s.accept()
        with conn:
            print('Connected by', addr)
            while True:
                data = conn.recv(1024)
                data = data.upper()
                if not data:
                    break
                conn.sendall(data)

def client():
    host = '127.0.0.1'
    port = 10000
    inputs = input("Input lowercase sentence:")
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host,port))
        s.sendall(inputs.encode())
        data = s.recv(1024)

    print('received',data.decode())

if __name__ == "__main__":
    client()