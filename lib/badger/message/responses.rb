module Badger
  module Message
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
  end
end
