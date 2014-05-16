module Badger
  module RPC
    module Net
      @sockets = Array.new

      def self.connect(host,port,local_host=nil,local_port=nil)
        socket = TCPSocket.new(host,port,local_host,local_port)

        @sockets[socket.fileno] = socket
        return socket.fileno
      end

      def self.listen(host=nil,port)
        socket = TCPServer.new(host,port)
        socket.listen(256)

        @sockets[socket.fileno] = socket
        return socket.fileno
      end

      def self.accept(fd)
          #socket = NetHelper.socket(fd)

        begin
          client = socket.accept_nonblock
          rescue IO::WaitReadable, Errno::EINTR
          return nil
        end

        @sockets[client.fileno] = client
        return client.fileno
      end

      def self.recv(fd)
          #socket = NetHelper.socket(fd)

        begin
          return socket.recv_nonblock(BLOCK_SIZE)
          rescue IO::WaitReadable
          return nil
        end
      end

      def self.send(fd,data)
         # NetHelper.socket(fd).send(data)
      end
    end
  end
end
















