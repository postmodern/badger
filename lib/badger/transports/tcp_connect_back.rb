module Badger
  module Transports
    class TCPConnectBack < Transport

      #
      # Establishes the connection.
      #
      def open
        @socket = TCPSocket.new(@host,@port)
      end

      #
      # Listens for requests.
      #
      def listen
        until @socket.closed?
          puts @socket.readline.chomp
        end
      end

      #
      # Closes the connection.
      #
      def close
        @socket.close
      end

    end
  end
end
