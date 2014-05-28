require 'socket'
require 'json'

def read(socket)
  while line = socket.gets
    puts line
    return JSON.parse(line.strip)
  end
end


def upload(path,socket)
  request = { :name => "Fs.open" , :arguments => ["./clientcopy","w+"] }.to_json
  socket.puts(request)
  response = read(socket)
  handle = response['return']
  data =File.read(path)
  request = { :name => "Fs.write" , :arguments => [handle,0,data] }.to_json
  socket.puts(request)
end

dest = ARGV[0]
port = ARGV[1].to_i
a = TCPSocket.open(dest, port)
upload('./clientcopy.rb',a)

#another example:  request = { :name => "Shell.execread" , :arguments => ["ls" , "-la"] }.to_json
#                  a.puts(request)















