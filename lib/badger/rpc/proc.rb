 module Badger
  module RPC
    module Proc
      def self.getpid
        return ::Process.pid
      end
      def self.getppid
        return ::Process.ppid
      end

        def self.kill(sig,pid)
        return ::Process.kill(sig,pid)
      end

      def self.time;                    Time.now.to_i;              end
      def self.spawn(program,*arguments)
        return system(program,*arguments)   #True or False
      end

      #def self.exit; exit; end
    end
  end
end
