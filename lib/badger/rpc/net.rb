module Badger
  module RPC
    module Net
      @sockets = Array.new

      def self.connect(host,port,local_host=nil,local_port=nil)
        socket = TCPSocket.new(host,port,local_host,local_port)

        @sockets[socket.fileno] = socket
        return socket.fileno
      end

      def self.listen(port,host=nil)
        socket = TCPServer.new(host || '0.0.0.0',port)
        socket.listen(256)

        @sockets[socket.fileno] = socket
        return socket.fileno
      end

      def self.accept(fd)
          #socket = NetHelper.socket(fd)

        begin
          client = socket.accept_nonblock
          return nil
        end

        @sockets[client.fileno] = client
        return client.fileno
      end

      def self.recv(fd)
          socket = @sockets[fd]

        begin
          return socket.recv_nonblock(1024)
          return nil
        end
      end

      def self.__send__(fd,data)
          @sockets[fd].send(data)
      end

    end
  end
end
















