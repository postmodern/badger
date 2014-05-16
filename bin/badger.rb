def usage
  puts <<EOF
usage: badger [OPTIONS]

  -S, --tcp-server [HOST:]PORT
  \tListens for messages on a given PORT and optional HOST.

  -C, --tcp-connect-back HOST:PORT
  \tConnects back to the given HOST and PORT.

  -V, --version
  \tPrints the version and exits.

  -h, --help
  \tPrints this cruft.
EOF
end

def error(message)
  $stderr.puts "badger: #{message}"
end

def fail(message)
  error(message)
  exit(-1)
end

transport = nil

while (opt = ARGV.shift)
  case opt
  when '-S', '--tcp-server'
    unless ARGV[0]
      fail("#{opt} requires an argument")
    end

    arg = ARGV.shift

    if arg.include?(':')
      host, port = arg.split(':',2)
      port = port.to_i
    else
      host = nil
      port = arg.to_i
    end

    transport = Badger::Transports::TCPServer.new(host,port)
  when '-c', '--tcp-connect-back'
    unless ARGV[0]
      fail("#{arg} requires an argument")
    end

    arg = ARGV.shift

    unless arg.include?(':')
      fail("#{opt} must be in format HOST:PORT")
    end

    host, port = ARGV[0].split(':')
    port = port.to_i

    transport = Badger::Transports::TCPConnectBack.new(host,port)
  when '-V', '--version'
    puts "badger #{Badger::VERSION}"
    exit
  when '-h', '--help'
    usage
    exit
  when nil
    $stderr.puts "badger: no options specified"
    exit(-1)
  else
    $stderr.puts "badger: unknown option: #{arg}"
    exit(-1)
  end
end

unless transport
  fail("must specify --tcp-server or --tcp-connect-back")
end

begin
  transport.open
  transport.listen
rescue Exception => error
  $stderr.puts error.message
  transport.close
end
