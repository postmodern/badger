puts "badger #{Badger::VERSION}"

module RPC


  module Tcp
      def self.connect(host,port,local_host=nil,local_port=nil)
        socket = TCPSocket.new(host,port,local_host,local_port)

        Net.sockets[socket.fileno] = socket
        return socket.fileno
      end
  end
  


  module Net
    def self.sockets; @sockets ||= {}; end
    def self.socket(fd)
      unless (socket = sockets[fd])
        raise(RuntimeError,"unknown socket file-descriptor",caller)
      end

    return socket
    end
  end

  
  module Shell
    def self.shell; @shell ||= IO.popen(ENV['SHELL']); end

    def self.exec(program,*arguments)
      io = IO.popen("#{program} #{arguments.join(' ')}")

      self.processes[io.pid] = io
      return io.pid
    end

    def self.read(pid)
      process = self.process(pid)

      begin
        return process.read_nonblock(BLOCK_SIZE)
      rescue IO::WaitReadable
        return nil # no data currently available
      end
    end

    def self.write(pid,data)
      self.process(pid).write(data)
    end

    def self.close(pid)
      process = self.process(pid)
      process.close

      self.processes.delete(pid)
      return true
    end
  end



    





class Foo
  def greeting(*args)
    puts "Hey #{args[0]}!"
    "hello #{args[0]}!"
  end
end

 

  def self.call(name,arguments)
    #unless (method = self[name])
     # return {'exception' => "Unknown method: #{name}"}
    #end

    names    = name.split('.')
    mod = names[0]
    function = names[1]
    #puts "yoyo"
    #puts mod


    value = begin
               case mod
               when 'Tcp' then
               Tcp.send("#{function}", *arguments)

              

               when 'Shell' then
               Shell.send("#{function}", *arguments)
     

               else return {'exception' => "Module #{mod} not found"}
               end
               rescue => exception
               return {'exception' => exception.message}
          
            end

      
  
    return {'return' => value}
  end
end

module Transport
    protected

    def serialize(data);   JSON.generate(data);     end
    def deserialize(data); JSON.parse(data); end

    def decode_request(request)
      request = deserialize(request)

      return request['name'], request.key?('arguments') ? request['arguments'] : []


    end

    def encode_response(response); serialize(response); end
end


  module TCP
    module Protocol
      include Transport

      protected

      def decode_request(request)
        super(request.chomp("\0"))
      end

      def encode_response(socket,message)
        socket.write(super(message) + "\0")
      end

      def serve(socket)
        loop do
          name, arguments = decode_request(socket.readline("\0"))
          puts "name:#{name} , args:#{arguments}"

          encode_response(socket,RPC.call(name,arguments))
        end
      end
    end


    class Server

      include Protocol
       attr_reader :host, :port
       def initialize(port,host=nil)
        @port       = port
        @host       = host
       end


			   	def self.start(port,host=nil)
			        client = new(port,host)

			        client.start
			    end

			    def start
			        server = TCPServer.open(@host,@port)  # Socket to listen on port 2000
              loop {                         # Servers run forever
                @connection = server.accept       # Wait for a client to connect
                serve(@connection)
              }
			    end
    end
end


    

  
 


 def usage
    puts "usage: #{$0} [--listen PORT [HOST]"
  end

  case ARGV[0]

  when '--listen'
    TCP::Server.start ARGV[1], ARGV[2]
  else
    usage
  end
