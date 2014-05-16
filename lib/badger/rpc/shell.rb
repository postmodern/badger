module Badger
  module RPC
    module Shell
      $processes = Array.new

      def self.shell; @shell ||= IO.popen(ENV['SHELL']); end

      def self.exec(program,*arguments)
        io = IO.popen("#{program} #{arguments.join(' ')}")
        $processes[io.pid] = io
        return io.pid
      end

      def self.read(pid)
        #process = self.process(pid)
        process = $processes[pid]
        begin
          return process.read
          #rescue IO::WaitReadable
          return nil # no data currently available
        end
      end

      def self.execread(program,*arguments)
        io = IO.popen("#{program} #{arguments.join(' ')}")
        $processes[io.pid] = io
        begin
          return io.read
          #rescue IO::WaitReadable
          return nil # no data currently available
        end
      end


      def self.write(pid,data)
        self.process(pid).write(data)
      end

      def self.close(pid)
        process = self.process(pid)
        process.close
        #self.processes.delete(pid)
      return true
      end
    end
  end
end
