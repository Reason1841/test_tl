void setAfterimages(uint8_t _afterImages) {
    m_numberOfAfterImages = _afterImages;
    m_hasAfterImage = true;
}

void removeAfterimages(){
    m_numberOfAfterImages = 0;
    m_hasAfterImage = false;
}