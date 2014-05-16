 module Badger
  module RPC
    module Fs
      def self.open(path,mode); File.new(path,mode).fileno; end

        def self.read(fd,position)
          file = File.for_fd(fd)
          file.seek(position)

          return (file.read(BLOCK_SIZE) || '')
        end

          def self.write(fd,position,data)
            file = File.for_fd(fd)
            file.seek(position)

            return file.write(data)
          end

          def self.seek(fd,position)
            file = File.for_fd(fd)
            file.seek(position)

            return file.pos
          end

          def self.close(fd); file = File.for_fd(fd).close; end

          def self.readlink(path);           File.readlink(path);                end
          def self.mktemp(basename);         Tempfile.new(basename).path;        end
          def self.unlink(path);             File.unlink(path);                  end
          def self.new(name,mode)            File.new(name,mode)                 end
          def self.delete(name)              File.delete(name)                   end
          def self.realpath(path)            File.realpath(path)                 end

          def self.stat(path);               File.stat(path);                    end
          def self.compare(path,other_path); File.compare_file(path,other_path); end
  end
end
end
