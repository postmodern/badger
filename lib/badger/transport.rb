module Badger
  class Transport

    # Transport host
    #
    # @return [String]
    attr_reader :host

    # Transport port
    #
    # @return [Integer]
    attr_reader :port

    #
    # Initializes the transport.
    #
    # @param [String] host
    #
    # @param [Integer] port
    #
    def initialize(host,port)
      @host = host
      @port = port
    end

    def open
    end

    #
    # @abstract
    #
    def listen
      raise(NotImplementedError,"#{self.class}##{__method__} not implemented")
    end

    def close
    end

  end
end
