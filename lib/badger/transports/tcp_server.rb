module Badger
  module Transports
    class TCPServer

      MAX_CLIENTS = 10

      attr_reader :port, :host

      def initialize(port,host=nil)
        @port = port
        @host = host

        @clients = {}
      end

      def open
        @server = ::TCPServer.new(@host,@port)
      end

      def listen
        until @server.closed?
          fds = [@server] + @clients.values.map { |client| client.socket }
          readable, writable, errored = IO.select(fds, [], fds)

          if errored.include?(@server)
            close
            break
          end

          errored.each do |socket|
            disconnect(socket)
          end

          if readable.include?(@server)
            readable.delete(@server)
            connect
          end

          readable.each do |socket|
            @clients[socket.to_i].read_line do |line|
              puts line
            end
          end
        end
      end

      def close
        @clients.each_value { |client| client.close }
        @clients.clear
        @server.close
      end

      protected

      class Client

        attr_reader :socket

        def initialize(socket)
          @socket = socket
          @buffer = ""
        end

        def id
          @socket.to_i
        end

        def read
          if (data = @socket.recv(4096))
            @buffer << data
          end
        end

        def read_line
          read

          if @buffer.include?("\n")
            line, rest = @buffer.split("\n",2)
            @buffer = rest

            yield line
          end
        end

        def write(data)
          @socket.write(data)
        end

        def close
          @socket.close
        end

      end

      def connect
        socket = @server.accept
        client = Client.new(socket)

        @clients[client.id] = client
      end

      def disconnect(socket)
        if (client = @clients.delete(client.id))
          client.close
        end
      end

    end
  end
end
