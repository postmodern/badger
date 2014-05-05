module Badger
  module Transports
    class TCPServer < Transport

      DEFAULT_HOST = '0.0.0.0'

      # The optional host to listen on.
      #
      # @return [String, DEFAULT_HOST]
      attr_reader :host

      #
      # Initializes the TCPServer transport.
      #
      # @param [String, nil] host
      #   The optional host to listen on. Defaults to {DEFAULT_HOST}.
      #
      # @param [Integer] port
      #   The port to listen on.
      #
      def initialize(host,port)
        super(host || DEFAULT_HOST, port)

        @clients = {}
      end

      #
      # Opens the transport.
      #
      def open
        @server = ::TCPServer.new(@host,@port)
      end

      #
      # Listens for messages.
      #
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
              puts"heeey:#{line}"
              
              response = Badger::RPC::Serve(line)
              @clients[socket.to_i].write(response)

            end
          end
        end
      end

      #
      # Closes the transport.
      #
      def close
        @clients.each_value { |client| client.close }
        @clients.clear
        @server.close
      end

      protected

      class Client

        NL = "\n"

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

          if (idx = @buffer.index(NL))
            line = @buffer[0, idx+1]
            @buffer = @buffer[idx+1, @buffer.length]

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
