module Badger
  module Message
    class InvalidMessage < RuntimeError
    end

    module Responses
      #
      # Returns a return response message.
      #
      # @param [Object] value
      #   The return value.
      #
      # @return [Hash]
      #   The response message.
      #
      def self.return(value)
        {'return' => value}
      end

      #
      # Returns an exception response message.
      #
      # @param [Exception] error
      #   The exception.
      #
      # @return [Hash]
      #   The response message.
      #
      def self.exception(error)
        {'exception' => error.message}
      end
    end

    #
    # Encodes the message.
    #
    # @param [Hash] message
    #   The message to encode.
    #
    # @return [String]
    #   The JSON encoded mssage.
    #
    def self.encode(message)
      JSON.generate(message) + "\n"
    end

    #
    # Decodes the message.
    #
    # @param [String] message
    #   The JSON encoded message.
    #
    # @return [Hash]
    #   The decoded message.
    #
    def self.decode(data)
      message = begin
                  JSON.parse(data.chomp)
                rescue JSON::ParserError => error
                  raise(InvalidMessage,error.message,error.backtrace)
                end

      unless message.kind_of?(Hash)
        raise(InvalidMessage,"message was not a Hash")
      end

      unless message.has_key?('name')
        raise(InvalidMessage,"message was missing the 'name' field")
      end

      unless message['name'].kind_of?(String)
        raise(InvalidMessage,"'name' was not a String")
      end

      unless message.has_key?('arguments')
        raise(InvalidMessage,"message was missing the 'arguments' field")
      end

      unless message['arguments'].kind_of?(Array)
        raise(InvalidMessage,"'arguments' was not an Array")
      end

      return message
    end
  end
end
