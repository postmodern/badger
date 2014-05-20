 module Badger
  module RPC
    module Fs
      @files = Array.new
      def self.open(path,mode)
        fd = File.new(path,mode)
        @files[fd.fileno] = fd
        return fd.fileno
      end

      def self.read(fileno,position)
        file = File.for_fd(@files[fileno])
        file.seek(position)

        return (file.read(2000) || '')
      end

      def self.write(fileno,position,data)
        file = File.for_fd(@files[fileno])
        file.seek(position)

        return file.write(data)
      end

      def self.seek(fileno,position)
        file = File.for_fd(@files[fileno])
        file.seek(position)

        return file.pos
      end

      def self.close(fileno); file = File.for_fd(@files[fileno]).close; end

      def self.readlink(path);           File.readlink(path);                end
      def self.mktemp(basename);         Tempfile.new(basename).path;        end
      def self.unlink(path);             File.unlink(path);                  end
      def self.new(name,mode)            File.new(name,mode)                 end
      def self.delete(name)              File.delete(name)                   end
      def self.realpath(path)            File.realpath(path)                 end

      def self.stat(path);               File.stat(path);                    end
  end
end
end
