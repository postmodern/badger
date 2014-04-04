def usage
  puts "usage: badger [OPTIONS]"
end

case ARGV[0]
when '-V', '--version'
  puts "badger #{Badger::VERSION}"
when '-h', '--help'
  usage
when nil
  $stderr.puts "badger: no options specified"
  exit -1
else
  $stderr.puts "badger: unknown option: #{ARGV[0]}"
  exit -1
end
