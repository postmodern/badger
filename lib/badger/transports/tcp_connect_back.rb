module Badger
  module Transports
    class TCPConnectBack

      attr_reader :host, :port

      def initialize(host,port)
        @host = host
        @port = port
      end

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
