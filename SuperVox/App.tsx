import React, {useEffect, useState} from 'react';
import {
  SafeAreaView,
  StatusBar,
  StyleSheet,
  Text,
  useColorScheme,
  View,
  NativeModules,
  TouchableOpacity,
  Switch,
} from 'react-native';
import Slider from '@react-native-community/slider';

const {AudioModule} = NativeModules;

function App(): React.JSX.Element {
  const isDarkMode = useColorScheme() === 'dark';
  const [isEngineRunning, setIsEngineRunning] = useState(false);
  const [nativeMessage, setNativeMessage] = useState('Loading...');
  const [focusStrength, setFocusStrength] = useState(1.0);

  useEffect(() => {
    if (AudioModule) {
      AudioModule.getNativeHello()
        .then((message: string) => setNativeMessage(message))
        .catch((err: any) => setNativeMessage('Error: ' + err.message));
    }
  }, []);

  const toggleEngine = async () => {
    if (!AudioModule) return;
    try {
      if (isEngineRunning) {
        AudioModule.stopEngine();
        setIsEngineRunning(false);
      } else {
        const success = await AudioModule.startEngine();
        if (success) {
          setIsEngineRunning(true);
          AudioModule.setFocusStrength(focusStrength);
        } else {
          console.error('Failed to start engine');
        }
      }
    } catch (e) {
      console.error(e);
    }
  };

  const onSliderChange = (value: number) => {
    setFocusStrength(value);
    if (AudioModule && isEngineRunning) {
      AudioModule.setFocusStrength(value);
    }
  };

  return (
    <SafeAreaView style={styles.container}>
      <StatusBar barStyle={isDarkMode ? 'light-content' : 'dark-content'} />
      <View style={styles.content}>
        <Text style={styles.title}>SuperVox</Text>
        <Text style={styles.subtitle}>Real-time AI Super-Hearing</Text>
        
        <View style={styles.controlBox}>
          <Text style={styles.label}>Engine Status</Text>
          <Switch
            trackColor={{false: '#767577', true: '#00E676'}}
            thumbColor={isEngineRunning ? '#fff' : '#f4f3f4'}
            onValueChange={toggleEngine}
            value={isEngineRunning}
          />
          <Text style={[styles.statusText, {color: isEngineRunning ? '#00E676' : '#FF5252'}]}>
            {isEngineRunning ? 'ACTIVE' : 'INACTIVE'}
          </Text>
        </View>

        <View style={styles.sliderContainer}>
          <View style={styles.sliderHeader}>
            <Text style={styles.label}>Focus Strength</Text>
            <Text style={styles.valueText}>{(focusStrength * 100).toFixed(0)}%</Text>
          </View>
          <Slider
            style={styles.slider}
            minimumValue={0}
            maximumValue={2.0}
            step={0.1}
            value={focusStrength}
            onValueChange={onSliderChange}
            minimumTrackTintColor="#00E676"
            maximumTrackTintColor="#333"
            thumbTintColor="#00E676"
          />
          <Text style={styles.sliderHint}>Lower to reduce background, Increase to amplify target.</Text>
        </View>

        <View style={styles.messageBox}>
          <Text style={styles.messageText}>{nativeMessage}</Text>
        </View>

        {isEngineRunning && (
          <View style={styles.visualizerContainer}>
             <Text style={styles.label}>[ Real-time Spectrogram ]</Text>
             <View style={styles.spectrogram}>
                {[...Array(20)].map((_, i) => (
                  <View 
                    key={i} 
                    style={[
                      styles.spectrogramBar, 
                      { 
                        height: 20 + Math.random() * 60,
                        backgroundColor: i > 15 ? '#FF5252' : '#00E676',
                        opacity: 0.3 + Math.random() * 0.7 
                      }
                    ]} 
                  />
                ))}
             </View>
          </View>
        )}
      </View>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#121212',
  },
  content: {
    flex: 1,
    alignItems: 'center',
    padding: 20,
    paddingTop: 40,
  },
  title: {
    fontSize: 48,
    fontWeight: 'bold',
    color: '#00E676',
    letterSpacing: 2,
  },
  subtitle: {
    fontSize: 18,
    color: '#B0BEC5',
    marginTop: 8,
    marginBottom: 30,
  },
  controlBox: {
    width: '100%',
    backgroundColor: '#1E1E1E',
    borderRadius: 16,
    padding: 20,
    flexDirection: 'row',
    alignItems: 'center',
    justifyContent: 'space-between',
    marginBottom: 20,
    borderWidth: 1,
    borderColor: '#333',
  },
  sliderContainer: {
    width: '100%',
    backgroundColor: '#1E1E1E',
    borderRadius: 16,
    padding: 20,
    marginBottom: 20,
    borderWidth: 1,
    borderColor: '#333',
  },
  sliderHeader: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    marginBottom: 10,
  },
  slider: {
    width: '100%',
    height: 40,
  },
  valueText: {
    color: '#00E676',
    fontSize: 20,
    fontWeight: 'bold',
  },
  sliderHint: {
    color: '#666',
    fontSize: 12,
    marginTop: 5,
    textAlign: 'center',
  },
  label: {
    fontSize: 18,
    color: '#FFF',
    fontWeight: '600',
  },
  statusText: {
    fontSize: 14,
    fontWeight: '800',
    width: 80,
    textAlign: 'right',
  },
  messageBox: {
    width: '100%',
    padding: 20,
    backgroundColor: '#1E1E1E',
    borderRadius: 12,
    borderWidth: 1,
    borderColor: '#333',
  },
  messageText: {
    fontSize: 14,
    color: '#999',
    fontFamily: 'monospace',
  },
  visualizerContainer: {
    width: '100%',
    marginTop: 20,
    alignItems: 'center',
  },
  spectrogram: {
    width: '100%',
    height: 120,
    backgroundColor: '#000',
    borderRadius: 8,
    marginTop: 12,
    padding: 10,
    flexDirection: 'row',
    alignItems: 'flex-end',
    justifyContent: 'space-between',
    borderWidth: 1,
    borderColor: '#333',
  },
  spectrogramBar: {
    width: '4%',
    borderRadius: 2,
  }
});

export default App;
