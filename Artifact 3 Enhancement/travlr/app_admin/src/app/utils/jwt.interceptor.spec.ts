import { TestBed } from '@angular/core/testing';
import { JwtInterceptor } from './jwt.interceptor';
import { AuthenticationService } from '../services/authentication.service';

describe('JwtInterceptor', () => {
  let interceptor: JwtInterceptor;
  let authServiceStub: Partial<AuthenticationService>;

  beforeEach(() => {
    authServiceStub = {
      isLoggedIn: () => false,
      getToken: () => ''
    };

    TestBed.configureTestingModule({
      providers: [
        JwtInterceptor,
        { provide: AuthenticationService, useValue: authServiceStub }
      ]
    });

    interceptor = TestBed.inject(JwtInterceptor);
  });

  it('should be created', () => {
    expect(interceptor).toBeTruthy();
  });
});
