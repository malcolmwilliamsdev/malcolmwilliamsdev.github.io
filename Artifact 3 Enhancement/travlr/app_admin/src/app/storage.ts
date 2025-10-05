import { InjectionToken } from '@angular/core';

export const BROWSER_STORAGE = new InjectionToken<Storage>('BrowserStorage', {
  providedIn: 'root',
  factory: () => localStorage,
});

export class Storage {}
