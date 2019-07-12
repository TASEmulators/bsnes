auto CPU::idle() -> void {
  status.irqLock = false;
  status.clockCount = 6;
  dmaEdge();
  step<6,0>();
  aluEdge();
}

auto CPU::readPC(uint address) -> uint8 {
  status.irqLock = false;

  if(address & 0x408000) {
    if(address & 0x800000 && io.fastROM) {
      status.clockCount = 6;
      dmaEdge();
      r.mar = address;
      step<6,0>();
    } else {
      status.clockCount = 8;
      dmaEdge();
      r.mar = address;
      step<8,0>();
    }
  } else if(address + 0x6000 & 0x4000) {
    status.clockCount = 8;
    dmaEdge();
    r.mar = address;
    step<8,0>();
  } else if(address - 0x4000 & 0x7e00) {
    status.clockCount = 6;
    dmaEdge();
    r.mar = address;
    step<6,0>();
  } else {
    status.clockCount = 12;
    dmaEdge();
    r.mar = address;
    step<12,0>();
  }

  auto data = bus.read(address, r.mdr);
  aluEdge();
  //$00-3f,80-bf:4000-43ff reads are internal to CPU, and do not update the MDR
  if((address & 0x40fc00) != 0x4000) r.mdr = data;
  return data;
}

auto CPU::read(uint address) -> uint8 {
  status.irqLock = false;

  if(address & 0x408000) {
    if(address & 0x800000 && io.fastROM) {
      status.clockCount = 6;
      dmaEdge();
      r.mar = address;
      step<2,1>();
    } else {
      status.clockCount = 8;
      dmaEdge();
      r.mar = address;
      step<4,1>();
    }
  } else if(address + 0x6000 & 0x4000) {
    status.clockCount = 8;
    dmaEdge();
    r.mar = address;
    step<4,1>();
  } else if(address - 0x4000 & 0x7e00) {
    status.clockCount = 6;
    dmaEdge();
    r.mar = address;
    step<2,1>();
  } else {
    status.clockCount = 12;
    dmaEdge();
    r.mar = address;
    step<8,1>();
  }

  auto data = bus.read(address, r.mdr);
  step<4,0>();
  aluEdge();
  //$00-3f,80-bf:4000-43ff reads are internal to CPU, and do not update the MDR
  if((address & 0x40fc00) != 0x4000) r.mdr = data;
  return data;
}

auto CPU::write(uint address, uint8 data) -> void {
  status.irqLock = false;
  aluEdge();

  if(address & 0x408000) {
    if(address & 0x800000 && io.fastROM) {
      status.clockCount = 6;
      dmaEdge();
      r.mar = address;
      step<6,1>();
    } else {
      status.clockCount = 8;
      dmaEdge();
      r.mar = address;
      step<8,1>();
    }
  } else if(address + 0x6000 & 0x4000) {
    status.clockCount = 8;
    dmaEdge();
    r.mar = address;
    step<8,1>();
  } else if(address - 0x4000 & 0x7e00) {
    status.clockCount = 6;
    dmaEdge();
    r.mar = address;
    step<6,1>();
  } else {
    status.clockCount = 12;
    dmaEdge();
    r.mar = address;
    step<12,1>();
  }

  bus.write(address, r.mdr = data);
}

auto CPU::readDisassembler(uint address) -> uint8 {
  return bus.read(address, r.mdr);
}
