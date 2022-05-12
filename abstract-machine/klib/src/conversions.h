#define CONVERSIONS(flag) switch(*fmt) \

                        numindex = 0;
                        while(isnum(*fmt)){
                            Num[numindex++] = *fmt++;
                        }
                        Num[numindex++] = '\n';
                        num = atoi(Num);


                        switch(*fmt++){
			    case 'i':
                            case 'd':
                                d = va_arg(ap, int);
                                if(d < 0){
                                    *out = '-';
                                    out ++;
                                    d = -d;
                                }
                                itoa(d, buf);
                                len = strlen(buf);
                                if(len >= num){
                                   memcpy(out, buf, len);
                                   out += strlen(buf);
                                }
                                else{
                                   memset(out, flag, num - len);
                                   out += num - len;
                                   memcpy(out, buf, len);
                                   out += len;
                                }
                                break;
                             case 'o':
                                d = va_arg(ap, int);
                                otoa(d, buf);
                                len = strlen(buf);
                                if(len >= num){
                                   memcpy(out, buf, len);
                                   out += len;
                                }
                                else{
                                   memset(out, flag, num - len);
                                   out += num - len;
                                   memcpy(out, buf, len);
                                   out += len;
                                }
                                break;
			     case 'e':
				dd = va_arg(ap, double)

                        }
