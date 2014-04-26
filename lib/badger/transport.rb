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

    def process(message)
      request = begin
                  Message.parse(message)
                rescue Message::InvalidMessage
                  # ignore invalid messages
                  return
                end

      name      = request['name']
      arguments = request['arguments']

      response = begin
                   Message::Responses.return(RPC.call(name,*arguments))
                 rescue Exception => error
                   Message::Responses.exception(error)
                 end

      return Message.encode(response)
    end

  end
end
