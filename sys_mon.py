import serial
import psutil
import argparse
import serial.tools.list_ports
import serial.serialutil
import platform

def write_out(port = None, debug = False):
    cpu_load = psutil.cpu_percent(interval = 0.25)
    mem_load = psutil.virtual_memory().percent

    if port:
        try:            
            with serial.Serial(port, 9600) as ser:
                ser.write(("c%03d\r" % ((cpu_load/100.0)*120.0)).encode('utf-8'))
                ser.write(("m%03d\r" % ((mem_load/100.0)*118.0)).encode('utf-8'))
        except serial.serialutil.SerialException:
            print("Curse your sudden but inevitable betrayal!")

    if debug:
        print("CPU load: %0.3f, memory load: %0.3f\r\n"% (cpu_load, mem_load))


def autodetect():

    if platform.system() == 'Windows':
        print("Automatic detection of System Monitor not supported on Windows.")
        return None

    port_list = serial.tools.list_ports.comports()

    port = None

    for p in port_list:
        if (p.manufacturer == 'kitsune-denshi.net' and p.product == 'System Monitor'):
            port = p.device
            

    if port:
        print("System Monitor found on %s" % port)
    else:
        print("System Monitor not found.")

    return port


if __name__ == "__main__":


    parser = argparse.ArgumentParser()
    parser.add_argument("--port", help="Serial port, use 'auto' for automatic detection")
    parser.add_argument("--loop", action='store_const', const=True, help="Loops until stopped")
    parser.add_argument("--debug", action='store_const', const=True, help="Prints current stats")
    args = parser.parse_args()

    if args.loop:
        print("Press CTRL-C to stop")

    if args.port == 'auto' or (not args.port):
        args.port = autodetect()

    run = True

    try:
        while run:
            write_out(port = args.port, debug = args.debug)     

            if not args.loop:
                run = False

    except KeyboardInterrupt:
            run = False

