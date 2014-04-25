module Badger
  module Message
    class InvalidMessage < RuntimeError
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
    def self.decode(message)
      begin
        JSON.parse(message.chomp)
      rescue JSON::ParserError => error
        raise(InvalidMessage,error.message,error.backtrace)
      end
    end
  end
end
