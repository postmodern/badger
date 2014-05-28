module Badger
  module Transports
    class TCPServer < Transport

      # Default host to listen on
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
              response = Badger::RPC::Serve(line)
              @clients[socket.to_i].write(response + "\r\n")

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

      #
      # Represents a connects to the server.
      #
      class Client

        NL = "\n"

        # The client's socket.
        #
        # @return [TCPSocket]
        attr_reader :socket

        #
        # Initializes the client.
        #
        # @param [TCPSocket] socket
        #   The socket.
        #
        def initialize(socket)
          @socket = socket
          @buffer = ""
        end

        #
        # Unique identifier.
        #
        # @return [Integer]
        #
        def id
          @socket.to_i
        end

        #
        # Reads available data from the client.
        #
        def read
          if (data = @socket.recv(4096))
            @buffer << data
          end
        end

        #
        # Reads a line from the client.
        #
        # @yield [line]
        #   When a new-line character is received, a line is yielded.
        #
        # @yieldparam [String] line
        #   The latest line of input.
        #
        def read_line
          read

          if (idx = @buffer.index(NL))
            line = @buffer[0, idx+1]
            @buffer = @buffer[idx+1, @buffer.length]

            yield line
          end
        end

        #
        # Writes data to the client.
        #
        # @param [String] data
        #
        def write(data)
          @socket.write(data)
        end

        #
        # Closes the clients connection.
        #
        def close
          @socket.close
        end

      end

      #
      # Accepts a connection from a client.
      #
      def connect
        socket = @server.accept
        client = Client.new(socket)

        @clients[client.id] = client
      end

      #
      # Disconnects a client.
      #
      # @param [Client] client
      #   The client to disconnect.
      #
      def disconnect(client)
        if (client = @clients.delete(client.id))
          client.close
        end
      end

    end
  end
end
