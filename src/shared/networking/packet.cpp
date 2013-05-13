#include "packet.h"

int Packet::Encrypt(ByteBuffer par)
{
    ByteBuffer ciphertext;
    
    int ret=0;

    switch(mode)
    {
        case AES_MODE:
        {
            ret = AesEncrypt(par, (ByteBuffer)(*this), ciphertext);
        }
        break;
        case RSA_MODE:
        {
            string pub = (const char*) par.contents();
            ret = RsaEncrypt(pub, (ByteBuffer)(*this), ciphertext);
        }
        break;
        default:
            INFO("debug", "PACKET: wrong enc type\n");
        break;
    }
    
    ciphertext.hexlike();
    
    if (!ret)
    {
        INFO("debug", "PACKET: encrypted\n");
        m_encrypted = true;
        this->clear();
        if (ciphertext.size())
            this->append(ciphertext.contents(), ciphertext.size());
    }
    else
        INFO("debug", "PACKET: encryption failed\n");
    
    return ret;
}

int Packet::Decrypt(ByteBuffer par)
{
    ByteBuffer *ciphertext = (ByteBuffer*) this;
    ByteBuffer plaintext;
    int ret=0;

    INFO("debug", "PACKET: decrypting\n");
    ciphertext->hexlike();

    switch(mode)
    {
        case AES_MODE:
        {
            ret = AesDecrypt(par, *ciphertext, plaintext);
        }
        break;
        case RSA_MODE:
        {
            string pwd, priv;
            par >> pwd;
            par >> priv;

            ret = RsaDecrypt(priv, pwd.c_str(), (ByteBuffer)(*this), plaintext);
        }
        break;
        default:
            INFO("debug", "PACKET: wrong enc type\n");
        break;
    }
  
    if (!ret)
    {
        INFO("debug", "PACKET: decrypted\n");
        m_encrypted = false;
        this->clear();
        this->append(plaintext);
    }
    else
        INFO("debug", "PACKET: decryption failed\n");
     
    return ret;
}

void Packet::Incapsulate(Packet& pkt)
{
    *this << uint16(pkt.GetOpcode());
    *this << uint16(pkt.size());
    INFO("debug","PACKET: packet incapsulate [header opcode %d, length %d]\n", pkt.GetOpcode(), pkt.size());
    if (pkt.size())
        this->append(pkt.contents(), pkt.size());
}

Packet* Packet::Decapsulate()
{    
    if (empty())
        return NULL;

    uint16 opcode; 
    uint16 size;
    *this >> opcode;
    *this >> size;

    INFO("debug","PACKET: packet decapsulate [header opcode %d, length %d]\n", opcode, size);

    Packet* new_pkt = new Packet(opcode, size);
    if (!new_pkt)
        return NULL;
    
    if (size)
    {
        new_pkt->append(contents()+rpos(), size);
        read_skip(size);
    }
    
    //INFO("debug","PACKET: packet decapsulate [contents %s]\n", new_pkt->contents());

    return new_pkt;
}
