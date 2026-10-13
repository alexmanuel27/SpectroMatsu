"""Run KiCad ERC and verify critical connections. Design: Ing. Alex M. Rivera."""
from pathlib import Path
import json
import shutil
import subprocess
import tempfile
import xml.etree.ElementTree as ET

folder = Path(__file__).resolve().parent
cli = shutil.which('kicad-cli') or '/Applications/KiCad/KiCad.app/Contents/MacOS/kicad-cli'
schematic = folder / 'spectromatsu.kicad_sch'
with tempfile.TemporaryDirectory() as tmp:
    tmp = Path(tmp)
    for args in [
        ['sch', 'erc', '--format', 'json', '-o', str(tmp / 'erc.json')],
        ['sch', 'export', 'netlist', '--format', 'kicadxml', '-o', str(tmp / 'nets.xml')],
    ]:
        run = subprocess.run([cli, *args, str(schematic)], capture_output=True, text=True)
        if run.returncode:
            raise RuntimeError(run.stdout + run.stderr)
    violations = [v for s in json.loads((tmp / 'erc.json').read_text())['sheets']
                  for v in s['violations']]
    assert not violations, violations
    tree = ET.parse(tmp / 'nets.xml')
    nets = {(node.get('ref'), node.get('pin')): net.get('name')
            for net in tree.findall('./nets/net') for node in net.findall('node')}
    def net(ref, pin):
        return nets[(ref, str(pin))]
    def joined(*pins):
        assert len({net(*pin) for pin in pins}) == 1, pins
    for pin in range(1, 17):
        joined(('J1', pin), ('J2', pin))
    assert net('U2', 1) == net('U2', 3) == 'ANALOG_5V'
    assert net('U2', 2) == net('U2', 5) == 'GND'
    assert net('U2', 8).startswith('unconnected-')
    joined(('U2', 10), ('U5', 1))  # video enters the non-inverting input
    joined(('U5', 3), ('U5', 4), ('R1', 1))  # negative feedback
    joined(('U2', 7), ('U1', 17))  # TRG is an output, not a gain input
    assert net('U1', 10) == net('A1', 9) == 'SHIFT_EN'
    joined(('R1', 2), ('R2', 1), ('R5', 1), ('U10', 4))
    joined(('R5', 2), ('A1', 1), ('D1', 2), ('D2', 1))
    assert net('D1', 1) == '3V3' and net('D2', 2) == 'GND'
    assert net('A1', 14).startswith('unconnected-')  # no backfeed through 5V pad
    assert net('U7', 7) == 'USB_5V' and net('U7', 5) == 'GND'
    joined(('U7', 8), ('R4', 1))
    assert net('U7', 6) == 'LED_EN'
    joined(('U11', 8), ('C24', 1))
    joined(('C24', 2), ('L2', 2), ('U11', 4), ('U11', 5), ('U11', 6), ('U11', 7))
    assert net('L2', 1) == 'USB_5V'
    joined(('U11', 17), ('R20', 2), ('R21', 1))
    joined(('U9', 7), ('R26', 1), ('C28', 1))
    assert net('U9', 9) == net('U9', 10) == 'ANALOG_5V'
    assert net('R26', 2) == net('R27', 2) == 'GND'
    assert net('U10', 8) == '3V3'  # ADS1115 is only a slow diagnostic channel
print('PASS: 0 ERC violations; stack, sensor, buffer, level shifter, LED and supply connections checked.')
print('This does not validate converter compensation, USB inrush, ADC timing, noise or PCB mechanics.')
