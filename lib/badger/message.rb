module Badger
  module Message
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
      JSON.generate(message)
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
      JSON.parse(data)
    end
  end
end
