module Badger
  module Transports
    class TCPConnectBack < Transport

      def open
        @socket = TCPSocket.new(@host,@port)
      end

      def listen
        until @socket.closed?
          puts @socket.readline.chomp
        end
      end

      def close
        @socket.close
      end

    end
  end
end
